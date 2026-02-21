/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:29:40 by ylang             #+#    #+#             */
/*   Updated: 2026/02/21 23:50:54 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
pipeline = build_cmd_list(node, sh)
把 echo "$X" 和 grep foo >out1 变成线性 list：[echo_node, redir(grep>out1)_node]
execute_pipeline_list(pipeline, sh)
循环：每个 stage 都 fork 子进程执行，并用 pipe 串接
*/

/*
execute_pipeline(...)        // 处理 AST_PIPELINE
 ├─ build_pipeline_stages()  // AST → [stage1, stage2, ...]
 ├─ for each stage:
 │    └─ spawn_pipeline_stage()
 └─ wait_for_pipeline_children(stage_count)
*/

// always build from right side
// ((A|B)|C) - > [A, B,C]
// t_ast_pipeline	*pipeline_list;
// 把树压平到一个CMD list
//? ?
t_list	*build_cmd_list(t_ast *node, t_shell_context *sh_ctx)
{
	t_list	*pipeline_list;

	pipeline_list = NULL;
	while (node->type == AST_PIPELINE)
	{
		lst_add_front_s(node->u_data.pipeline.right, &pipeline_list,
			ALLOC_PROMPT, sh_ctx);
		if (node->u_data.pipeline.left->type == AST_PIPELINE)
			node = node->u_data.pipeline.left;
		else
		{
			lst_add_front_s(node->u_data.pipeline.left, &pipeline_list,
				ALLOC_PROMPT, sh_ctx);
			break ;
		}
	}
	return (pipeline_list);
}
// return parent pid
// child
// if not the first cmd
// if not the last
// 执行这个 stage 的 AST，然后退出（O_EXIT）

int	execute_one_command(t_list *pipeline, int prev_read_end, int pipe[2],
		t_shell_context *sh_ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid > 0)
		return (pid);
	sh_ctx->in_main_process = false;
	set_signal_in_exe_child_process();
	if (prev_read_end != -1)
	{
		dup2_s(prev_read_end, STDIN_FILENO, sh_ctx);
		close_s(prev_read_end, sh_ctx);
	}
	if (pipeline->next != NULL)
		dup2_s(pipe[WRITE_END], STDOUT_FILENO, sh_ctx);
	if (pipe[READ_END] != -1)
		close_s(pipe[READ_END], sh_ctx);
	if (pipe[WRITE_END] != -1)
		close_s(pipe[WRITE_END], sh_ctx);
	status = execute((t_ast *)pipeline->content, RUN_IN_CHILD, sh_ctx);
	exit(status);
}

// setup a pipe if there is next command
static int	setup_pipe_for_stage(int pipe_fd[2], bool has_next)
{
	if (!has_next)
	{
		pipe_fd[READ_END] = -1;
		pipe_fd[WRITE_END] = -1;
		return (0);
	}
	if (pipe(pipe_fd) < 0)
		return (-1);
	return (0);
}

// clean up file descrittorws in the parent after forking a stage
static void	cleanup_parent_fds(int *prev_read_end, int pipe_fd[2],
		bool has_next)
{
	if (*prev_read_end != -1)
		close(*prev_read_end);
	if (has_next)
		close(pipe_fd[WRITE_END]);
	if (has_next)
		*prev_read_end = pipe_fd[READ_END];
	else
		*prev_read_end = -1;
}

// bool	new_line;
// new_line = false;
/*
wait(&status) 可能返回 -1 并设置 errno = EINTR。
在你项目里（父进程在执行阶段可能 ignore SIGINT/SIGQUIT，但仍可能被其它信号打断），这是实际会发生的。
*/
// only do ui output to the last cmd
// report_child_termination_signal(status, &new_line, sh_ctx);
// pipeline &? last cmd exit code
// ui output ()

int	wait_for_children(pid_t last_pid, int count_pipeline,
		t_shell_context *sh_ctx)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;

	last_cmd_status = 0;
	while (count_pipeline > 0)
	{
		child_pid = wait(&status);
		if (child_pid == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		count_pipeline--;
		if (child_pid == last_pid)
		{
			report_child_termination_signal(status, NULL, sh_ctx);
			last_cmd_status = wait_status_to_shell_status(status);
		}
	}
	return (last_cmd_status);
}

// last cmd
// parent closes
// close write end in parent
int	execute_pipeline_commands(t_list *pipeline, t_shell_context *sh_ctx)
{
	int		pipe_fd[2];
	int		prev_read_end;
	pid_t	last_pid;
	t_list	*stage;
	int		last_cmd_status;

	last_pid = 0;
	prev_read_end = -1;
	stage = pipeline;
	while (stage)
	{
		if (setup_pipe_for_stage(pipe_fd, stage->next != NULL) < 0)
			return (print_errno_n_return(1, "pipe", NULL, errno));
		last_pid = execute_one_command(stage, prev_read_end, pipe_fd, sh_ctx);
		cleanup_parent_fds(&prev_read_end, pipe_fd, stage->next != NULL);
		stage = stage->next;
	}
	if (prev_read_end != -1)
		close(prev_read_end);
	last_cmd_status = wait_for_children(last_pid, ft_lstsize(pipeline), sh_ctx);
	ft_lstclear(&pipeline, NULL);
	return (wait_for_children(last_pid, ft_lstsize(pipeline), sh_ctx));
}

int	execute_pipeline(t_ast *pipeline_node, t_shell_context *sh_ctx)
{
	t_list	*pipeline_list;

	if (!pipeline_node)
		return (EXIT_FAILURE);
	pipeline_list = build_cmd_list(pipeline_node, sh_ctx);
	return (execute_pipeline_commands(pipeline_list, sh_ctx));
}

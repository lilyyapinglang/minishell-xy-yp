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
t_ast_pipeline	*build_cmd_list(t_ast *node, t_shell_context *sh_ctx)
{
	t_ast_pipeline	*pipeline_list;

	// 把树压平到一个CMD list
	pipeline_list = NULL;
	while (node->type == AST_PIPELINE)
	{
		lst_add_front_s(node->u_data.pipeline.right, &pipeline_list,
			ALLOC_PROMPT, sh_ctx);
		if (node->u_data.pipeline.left->type == AST_PIPELINE)
			node = node->u_data.pipeline.left;
		else
		{
			lst_add_front_s(node->u_data.pipeline.left, &pipeline_list, sh_ctx);
			break ;
			//? ?
		}
	}
	return (pipeline_list);
}

int	execute_one_command(t_list *pipeline, int prev_read_end, int pipe[2],
		t_shell_context *sh_ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid > 0)
		return (pid); // return parent pid
	// child
	sh_ctx->in_main_process = false;
	set_signal_in_exe_child_process();
	// if not the first cmd
	if (prev_read_end != -1)
	{
		dup2_s(prev_read_end, STDIN_FILENO, sh_ctx);
		close_s(prev_read_end, sh_ctx);
	}
	// if not the last
	if (pipeline->next != NULL)
		dup2_s(pipe[WRITE_END], STDOUT_FILENO, sh_ctx);
	if (pipe[READ_END] != -1)
		close(pipe[READ_END]);
	if (pipe[WRITE_END] != -1)
		close(pipe[WRITE_END]);
	// 执行这个 stage 的 AST，然后退出（O_EXIT）
	status = execute((t_ast *)pipeline->content, RUN_IN_CHILD, sh_ctx);
	exit(status);
	return (pid);
}

int	wait_for_children(pid_t last_pid, int count_pipeline,
		t_shell_context *sh_ctx)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;
	bool	new_line;

	new_line = false;
	last_cmd_status = 0;
	while (count_pipeline > 0)
	{
		child_pid = wait(&status);
		/*
		wait(&status) 可能返回 -1 并设置 errno = EINTR。
在你项目里（父进程在执行阶段可能 ignore SIGINT/SIGQUIT，但仍可能被其它信号打断），这是实际会发生的。
		*/
		if (child_pid == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		count_pipeline--;
		if (child_pid == last_pid)
		{
			// only do ui output to the last cmd
			report_child_termination_signal(status, &new_line, sh_ctx);
			// pipeline &? last cmd exit code
			last_cmd_status = wait_status_to_shell_status(status);
			// ui output ()
		}
	}
	return (last_cmd_status);
}

int	execute_pipeline_commands(t_list *pipeline, t_shell_context *sh_ctx)
{
	int		fd[2];
	int		prev_read_end;
	pid_t	last_pid;
	int		last_cmd_status;
	int		pipeline_count;
	bool	has_next;

	pipeline_count = ft_lstsize(pipeline);
	//
	last_pid = 0;
	prev_read_end = -1;
	while (pipeline)
	{
		has_next = (pipeline->next != NULL);
		if (has_next && pipe(fd) < 0)
			return (1);
		// last cmd
		if (!has_next)
		{
			fd[READ_END] = -1;
			fd[WRITE_END] = -1;
		}
		last_pid = execute_one_command(pipeline, prev_read_end, fd, sh_ctx);
		// parent closes
		if (prev_read_end != -1)
			close(prev_read_end);
		if (has_next)
			close(fd[WRITE_END]); // close write end in parent
		if (has_next)
			prev_read_end = fd[READ_END];
		else
			prev_read_end = -1;
		pipeline = pipeline->next;
	}
	if (prev_read_end != -1)
		close(prev_read_end);
	last_cmd_status = wait_for_children(last_pid, pipeline_count, sh_ctx);
	return (last_cmd_status);
}

int	execute_pipeline(t_ast *pipeline_node, t_shell_context *sh_ctx)
{
	t_ast_pipeline *pipeline;

	pipeline = build_cmd_list(pipeline_node, sh_ctx);
	return (execute_pipeline_commands(pipeline, sh_ctx));
}
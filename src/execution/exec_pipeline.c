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
int	execute_pipeline(t_ast *pipeline_node, t_shell *shell)
{
	t_ast_pipeline	*pipeline;

	pipeline = build_cmd_list(pipeline_node, shell);
	execute_pipeline_commands(pipeline, shell);
}

// add a note to the front of a list
t_list			**ft_listadd_front(t_list **lst, t_list new_node);
{
}

// always build from right side
// ((A|B)|C) - > [A, B,C]
t_ast_pipeline	*build_cmd_list(t_ast *node, t_shell *shell)
{
	t_ast_pipeline	*pipeline_list;

	// 把树压平到一个CMD list
	pipeline_list = NULL;
	while (node->type == AST_PIPELINE)
	{
		ft_list_add_front(node->u_data.pipeline.right, &pipeline_list);
		if (node->u_data.pipeline.left->type == AST_PIPELINE)
			node = node->u_data.pipeline.left;
		else
		{
			ft_listadd_front(node->u_data.pipeline.left, &pipeline_list);
			break ;
			//? ?
		}
	}
	return (pipeline_list);
}

int	execute_pipeline_commands(t_list *pipeline, t_shell *shell)
{
	int		fd[2];
	int		prev_read_end;
	pid_t	last_pid;
	int		last_cmd_status;
	int		pipeline_count;

	pipeline_count = ft_lstsize(pipeline);
	//
	last_pid = 0;
	prev_read_end = -1;
	while (pipeline)
	{
		pipe(fd);
		last_pid = execute_one_command(pipeline, prev_read_end, pipe, shell);
		if (prev_read_end != -1)
			close(prev_read_end);
		close(pipe[WRITE_END]);
		prev_read_end = pipe[READ_END];
		pipeline = pipeline->next;
	}
	last_cmd_status = wait_for_children(last_pid, pipeline_count, shell);
}

int	execute_one_command(t_list *pipeline, int prev_read_end, int pipe[2],
		t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid != 0)
		return (pid); // return parent pid
	shell->in_main_process = false;
	set_signal_exe_child_process();
	// if not the first cmd
	if (prev_read_end != -1)
	{
		dup2_s(prev_read_end, STDIN_FILENO, shell);
		close_s(prev_read_end, shell);
	}
	// if not the last
	if (pipeline->next != NULL)
		dup2_s(pipe[WRITE_END], STDOUT_FILENO, shell);
	close(pipe[READ_END]);
	close(pipe[WRITE_END]);
	//执行这个 stage 的 AST，然后退出（O_EXIT）
	execute((t_ast *)pipeline->content, O_EXIT, shell);
	return (pid);
}

int	wait_for_children(pid_t last_pid, int count_pipeline, t_shell *shell)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;
	bool	new_line;

	new_line = false;
	last_cmd_status = 0;
	while (count_pipeline--)
	{
		child_pid = wait(&status);
		if (child_pid == last_pid)
			last_cmd_status = check_process_child_exit(status, &new_line,
					shell);
		else
			check_process_child_ecot(status, &new_line, shell);
	}
	return (last_cmd_status);
}

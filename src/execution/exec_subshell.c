#include "../inc/minishell.h"

/*
典型语义：
父进程（主 shell）：
pid = fork()
waitpid(pid, &st)
用 check_process_child_exit(st, ...) 得到 subshell 的 status 并返回给 AND
子进程（subshell child）：
执行括号内整棵树：execute(subshell_child_tree, O_EXIT, sh)
结束后必须 exit（由 O_EXIT 保证）
所以现在出现第一个 fork：

*/

// run a smaller ast tree in subshell.

int	wait_status_to_exit_code(int wait_status)
{
	int	signal_num;

	// turn waitstatus to $?
	// if child process exit normally
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(status));
	// child process ended/interrupeted by signal
	else if (WIFSIGNALED(wait_status))
	{
		signal_num = WTERMSIG(wait_status);
		return (128 + signal_num);
	}
	else
		return (1);
}
// when child process is terminated by signal, print user realted prompr ,
//	for exmaple new line ,quit etc
void	report_child_termination_signal(int wait_status, bool *new_line_existed,
		t_shell_context *shell_conetext)
{
	int	signal_num;

	if (!WIFSIGNALED(wait_status))
		return ;
	signal_num = WTERMSIG(wait_status);
	// 如果是 SIGQUIT，打印一条 Quit 信息
	/*
	对 SIGINT / SIGQUIT 补一个换行，并确保只补一次
	SIGINT（Ctrl-C）和 SIGQUIT（Ctrl-\）经常会让终端光标停在一行中间，为了让 prompt 下一行显示干净，shell 通常会补一个 \n。
但是 pipeline 里可能有多个子进程都因同一个信号结束，如果每个都补换行，就会出现多行空行。
	*/
	/* option 2
		if (sig == SIGQUIT)
		write(STDERR_FILENO, "Quit: 3", 7);
	if (sig == SIGQUIT || sig == SIGINT)
	{
		if (!new_line_existed || *new_line_existed == false)
			write(STDERR_FILENO, "\n", 1);
		if (new_line_existed)
			*new_line_existed = true;
	}
	*/
	if (sig == SIGQUIT)
		write(STDERR_FILENO, "Quit: 3\n", 8);
	else if (sig == SIGINT)
		write(STDERR_FILENO, "\n", 1);
}

int	execute_subshell(t_ast *node, t_shell_context *shell_conetext)
{
	pid_t			pid;
	int				status;
	t_ast_subshell	*subshell_node;

	subshell_node = node->u_data.subshell;
	pid = fork(); // fork parent process
	// child porcess execute
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		shell->in_main_process = false;
		// as long as i am in a forked chiled process,
		//	i need to consider signal.
		set_signal_in_exe_child_process();
		// child process need to exit otherwise we continue to run till waitpid
		status = execute(subshell_node->child, RUN_IN_CHILD, shell);
		exit(status);
	}
	// even if sigint and sigout is ignored during exection process,
	// waitpid could be
	// interrupted by other signal that leads to return
	//	-1 and errno set to EINTR
	// so if waitpid return -1, & errn=EINTR, continue waiting
	// other error , return error
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		return (1);
	}
	report_child_signal(status, &new_line, shell);
	status = wait_status_to_shell_status(status);
	return (status);
}

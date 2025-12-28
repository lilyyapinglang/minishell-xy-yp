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

// run a smaller ast tree in subshell.i

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

void	report_child_signal(int wait_status, bool *new_line_existed,
		t_shell *shell)
{
	int	signal_num;

	if (WIFSIGNALED(wait_status))
	{
		if (signal_num = SIGQUIT)
			write("Quit (core dumped)", STDERR_FILENO, 18);
		if (signal_num == SIGQUIT || signal_num == SIGINT)
		{
			if (!new_line_existed || (new_line_existed
					&& *new_line_existed == false))
				write("\n", STDERR_FILENO, 1);
			if (new_line_existed && *new_line_existed == false)
				*new_line_existed = true;
		}
	}
}

int	execute_subshell(t_ast_subshell *subshell_node, t_shell *shell)
{
	pid_t	pid;
	int		status;

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

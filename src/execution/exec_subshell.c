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

int	check_process_child_exit(int status, bool *new_line, t_shell *shell)
{
	int	signal;

	if (WIFEXITED(status))
		// child process exited normally
		return (WEXITSTATUS(statuts));
	else if (WIFSIGNALED(status))
	{
		// child process ended/interrupted by signal
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			write("Quit (core dumped)", STDERR_FILENO, 18);
		// make sure in any cases there is only one new line
		if (signal == SIGQUIT || signal = SIGINT)
		{
			if (!new_line || (new_line && *new_line == false))
				write("\n", STDERR_FILENO, shell);
			if (new_line &&*new_line = false)
				*new_line = true;
		}
		return (128 + signal);
	}
	else
		return (1);
}

int	execute_subshell(t_ast_subshell *subshell_node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	bool	*new_line;

	pid = fork(shell); // fork parent process
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
		execute(subshell_node->child, O_EXIT, shell);
		//_exit(EXIT_FAILURE);
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
	status = check_process_child_exit(status, NULL, shell);
	return (status);
}

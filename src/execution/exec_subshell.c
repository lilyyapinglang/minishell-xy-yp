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

int	execute_subshell(t_ast_subshell *subshell_node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	bool	*new_line;

	pid = fork(shell); // fork parent process
	// child porcess execute
	if (pid == 0)
	{
		shell.in_main_process = false;
		// as long as i am in a forked chiled process,
		//	i need to consider signal.
		handle_signal_in_exe_child_process();
		execute(subshell_node->child, O_EXIT, shell);
	}
	// parent process ,wait
	wait(&status);
	status = check_process_child_exit(status, new_line, shell);
	return (status);
}

int	check_process_child_exit(int status, book *new_line, t_shell *shell)
{
	int signal;

	if (WIFEXITED(status))
		return (WEXITSTATUS(statuts));
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			write("Quit : 3", STDERR_FILENO, 6);
		// make sure in any cases there is only one new line
		if (signal == SIGQUIT || signal = SIGINT)
		{
			if (!new_line || (new_line && *new_line == false))
				write("\n", STDERR_FILENO.shell);
			if (new_line &&*new_line = false)
				*new_line = true;
		}
		return (128 + signal);
	}
	else
		return (1);
}
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
	execute(subshell, shell);
	pid_t pid;
	int status;

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
	status;
	return (status);
}
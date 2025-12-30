#include "../inc/minishell.h"

/*

builtin 是否在父进程/子进程跑，取决于调用 execute_command() 的那个进程是谁？”
**如果当前进程是“主 shell”，
那么：

external command → 必须 fork

stateful builtin → 不能 fork

stateless builtin → 可 fork 可不 fork**
*/
int	execute_cmd(t_ast_command *cmd, t_exec_context exectuion_context,
		t_shell_context *shell_conetext)
{
	int		status;
	bool	isbuiltin;

	status = EXIT_SUCCESS;
	isbuiltin = false;
	// check if it is built-in
	// if it is built-in like cd, export, unset, ecit et,
	// run directly in currentg process
	// if not buitle-in , like ls , grep,
	// handover to fork_command decide how to run it
	// builtin 总是在“当前进程”执行（谁调用它，它就在哪个进程里跑）
	if (!cmd->args || !cmd->args[0])
		return (EXIT_SUCCESS);
	isbuiltin = get_builtin(cmd->args[0]);
	// already in child process, must not fork
	if (exectuion_context == RUN_IN_CHILD)
	{
		if (isbuiltin)
		{
			status = exec_builtin(cmd, shell);
			return (status);
		}
		// esle it is not build in, execve rewrite current process
		execve(get_cmd_path(cmd->args[0], shell), cmd->args, cpy_env, shell);
		return (127);
	}
	// has to be executer in parent process,shell process
	if (exectuion_context == RUN_IN_SHELL)
	{
		// stateful builti has to be run in parent ,
		//	otherwise change will not take effect
		if (isbuiltin && is_stateful_builtin(cmd->args[0]))
			return (exec_builtin(cmd, shell));
		if (isbuiltin) // stateless builtin , can be run in parent too,
						// i choose not to fork
			return (exec_builtin(cmd, shell));
		// external cmd is not allowed to run in parent,
		//	otherwise it will quit shell after executer
		exectuion_context = RUN_FORK_WAIT;
	}
	// need to fork , parent fork, child exe as run in child, parent wait
	return (fork_and_run_cmd_in_child(cmd, shell, isbuiltin));
}

int	fork_and_run_cmd_in_child(t_ast_command *cmd, t_shell_context *shell_conetext,
		t_built_in_func bi)
{
	pid_t pid;
	int wait_status;
	int status;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		shell->in_main_process = false;
		set_signal_child_process();
		/// reuse run in child logic
		status = execute_cmd(cmd, RUN_IN_CHILD, shell);
		exit(status);
	}
	while (waitpid(pid, &wait_status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		return (EXIT_FAILURE);
	}

	report_child_termination_signal(wait_status, NULL, shell);
	return (wait_status_to_shell_status(wait_status));
}
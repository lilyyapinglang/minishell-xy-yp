#include "../inc/minishell.h"

/*

builtin 是否在父进程/子进程跑，取决于调用 execute_command() 的那个进程是谁？”
*/
int	execute_cmd(t_ast_command *cmd, t_exitORreturnAfterCurExe exitoreturn,
		t_shell shell)
{
	int	status;

	status = 0;
	// check if it is built-in
	// if it is built-in like cd, export, unset, ecit et,
	// run directly in currentg process
	// if not buitle-in , like ls , grep,
	// handover to fork_command decide how to run it
	// builtin 总是在“当前进程”执行（谁调用它，它就在哪个进程里跑）
	if (isbuiltin)
		return (builtin(cmd, shell));
}

int	fork_command(t_ast_command *cmd, t_exitORreturnAfterCurExe exitoreturn,
		t_shell *shell)
{
	pid_t pid;
	int status;

	status = 0;
	// rethink about cases where O_EXIT was passed
	// it is when i am already in a child process exection,
	// either was called in child process of each pipeline stage,
	// or was called in the forked process of subshell
	// as i am already the child process, i will run execve to cover myself

	if (exitoreturn = O_EXIT) // child process was created
		execve(path, argv, env);
	else
	{ // O_RETURN was sent , no child proces was previously created,
		pid = fork();
		if (pid == 0)
		{
			shell->in_main_process = false;
			set_signal_child_process();
			execve(path, argv, env);
		}
		wait(&status);
		status = check_process_child_exit(status, NULL, shell);
	}
	return (status);
}
#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell_context shell_context;

	int exit_status;

	(void)argv;
	if (argc != 1)
		exit(EXIT_FAILURE);
	// current at main process, main prompt, set ctrl-c, ctrl-d , ctrl-\-
	// set signal first
	/*配置“主进程”的信号行为
	prompt 阶段 Ctrl-C / Ctrl-\ 的行为?
	*/
	// interactive shell setup (signals, state)
	set_signal_in_exe_main_process();
	init_shell(&shell_context, envp);
	// Shell main loop (REPL: Read–Eval–Print Loop)
	exit_status = shell_repl_loop(&shell_context);
	// final cleanup and exit
	shell_destroy(exit_status, &shell_context);
}
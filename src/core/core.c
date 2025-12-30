#include "../inc/minishell.h"

void	init_shell(t_shell_context *shell_context, char **envp)
{
	errno = 0;
	shell_context->in_main_process = true;
	shell_context->last_status = EXIT_SUCCESS;
	shell_context->allocated_pointers[TRACK_SHELL] = NULL;
	shell_context->allocated_pointers[TRACK_CYCLE] = NULL;
	// allocated_pointers[TRACK_NONE] is unused because ONLY_CHECK allocations are not tracked
	shell_context->temporary_files = NULL;
	shell_context->env = init_env(envp, shell_context);
	increment_shlvl(shell_context);
	shell_context->parsing_error = NULL;
}
// Shell main loop (REPL: Read–Eval–Print Loop).
int	shell_repl_loop(t_shell_context *shell_context)
{
	char	*line;

	while (1)
	{
		g_latest_signal_status = 0;
		line = prompt_listener("MAIN_PROMPT");
		// ctrl-c pressed at prompt: handler sets g_singal_value
		if (g_latest_signal_status = SIGINT)
			shell_context->last_status = 130;
		if (!line)
			quit_shell(shell_context->last_status, shell_context);
		track_alloc(line, TRACK_CYCLE, shell_context);
		if (line[0] != '\0')
		{
			add_history(line);
			shell_context->last_status = prompt_execution(line, shell_context);
		}
		clear_cycle(shell_context);
	}
}
void	quit_shell(int exit_status, t_shell_context *shell_context)
{
	if (shell_context)
	{
		if (shell_context->in_main_process && isatty(STDIN_FILENO))
			ft_putendl_fd("exit\n", STDERR_FILENO);
		clear_cycle(shell_context);
		// clear all t_list shell_context->env
		// clear t_list shell_context->allocated_pointers[TRACK_SHELL]
	}
	rl_clear_history();
	//?why exit status
	exit(exit_status);
}
// increment shell level at each initiation
void	increment_shlvl(t_shell_context *shell_context)
{
}

int	prompt_execution(char *line, t_shell_context *shell_context)
{
	t_list	*token_list;
	t_ast	*ast;
	int		status;

	// TODO
	// need to add parser ;
	// if parser result suceefsullt get a ast tree
	// start exection
	status = collect_heredocs(ast, shell_context);
	if (status == EXIT_SUCCESS)
	{
		status = execute(ast, RUN_IN_SHELL, shell_context);
	}
	return (status);
}

/*non interactive input handler for testing purpose*/
char	*non_interactive_input(void)
{
}
char	*prompt_listener(t_prompt_mode mode)
{
	char	*line;

	line = NULL;
	//每次开始 read 输入之前 reset signal
	g_latest_signal_status = 0;
	if (isatty(STDIN_FILENO))
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		if (mode = "MAIN_PROMPT")
		{
			set_signal_main_prompt();
			line = readline();
			set_signal_exe_main_process();
		}
		else if (mode == "HEREDOC_PROMPT")
		{
			set_signal_heredoc_prompt();
			line = readline("> ");
			set_signal_exe_main_process();
		}
	}
	else
		line = non_interactive_input();
	return (line);
}
/*
clear heredoc
cycle allocations
parsing_error
*/
void	clear_cycle(t_shell_context *shell_context)
{
	while (shell_context->in_main_process && shell_context->temporary_files)
	{
		if (unlink(shell_context->temporary_files->content) == -1)
			error("unlink");
		shell_context->temporary_files = shell_context->temporary_files->next;
	}
	// ft_lstclear(t_list **, free)
	ft_lstclear(&(shell_context->allocated_pointers[TRACK_CYCLE]), free);
	shell_context->parsing_error = NULL;
}

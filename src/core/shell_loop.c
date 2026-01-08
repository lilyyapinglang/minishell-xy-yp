#include "../inc/minishell.h"

/*non interactive input handler for testing purpose*/
// TODO:
char	*non_interactive_input(void)
{
	return (NULL);
}
// TODO
// void	track_alloc(char *line, t_tracking_scope scope,
// 		t_shell_context *sh_ctx)
// {
// 	(void)line;
// 	(void)scope;
// 	(void)sh_ctx;
// }

char	*prompt_listener(t_prompt_mode mode)
{
	char	*user_input;

	user_input = NULL;
	// 每次开始 read 输入之前 reset signal
	g_latest_signal_status = 0;
	errno = 0;
	if (!isatty(STDIN_FILENO))
	{
		// rl_replace_line("", 0);
		// rl_on_new_line();
		if (mode == MAIN_PROMPT)
		{
			set_signal_in_main_prompt_mode();
			user_input = readline("minishell >");
			set_signal_in_exe_main_process();
		}
		else if (mode == HEREDOC_PROMPT)
		{
			set_signal_in_heredoc_prompt_mode();
			user_input = readline("> ");
			set_signal_in_exe_main_process();
		}
	}
	else
		user_input = non_interactive_input();
	return (user_input);
}

int	prompt_execution(char *user_input, t_shell_context *sh_ctx)
{
	t_ast	*ast;
	t_list	*token_list;
	int		status;

	status = lexer(user_input, &token_list, sh_ctx);
	if (status == EXIT_SUCCESS && token_list)
	{
		status = parser(token_list, &ast, sh_ctx);
		if (status == EXIT_SUCCESS && ast)
		{
			status = collect_all_heredocs(ast, sh_ctx);
			if (status == EXIT_SUCCESS)
				status = execute(ast, RUN_IN_SHELL, sh_ctx);
		}
	}
	return (status);
}

// Shell main loop (REPL: Read–Eval–Print Loop).
int	shell_repl_loop(t_shell_context *sh_ctx)
{
	char	*user_intput;

	while (1)
	{
		user_intput = prompt_listener(MAIN_PROMPT);
		// ctrl-c pressed at prompt: handler sets g_singal_value
		if (g_latest_signal_status == SIGINT)
		{
			sh_ctx->last_status = 130;
			/* readline may return NULL or "" depending on behavior */
			if (user_intput)
				track_alloc(user_intput, ALLOC_PROMPT, sh_ctx);
			shell_clear_iteration(sh_ctx);
			continue ;
		}
		/* Ctrl-D / EOF , user_input == NULL */
		if (!user_intput)
			shell_exit(sh_ctx, sh_ctx->last_status);

		/*normal line */
		track_alloc(user_intput, ALLOC_PROMPT, sh_ctx);
		if (user_intput[0] != '\0')
		{
			add_history(user_intput);
			sh_ctx->last_status = prompt_execution(user_intput, sh_ctx);
		}
		shell_clear_iteration(sh_ctx);
	}
	return (sh_ctx->last_status);
}



#include "../inc/minishell.h"


// TODO
// void	track_alloc(char *line, t_tracking_scope scope,
// 		t_shell_context *sh_ctx)
// {
// 	(void)line;
// 	(void)scope;
// 	(void)sh_ctx;
// }

// Shell main loop (REPL: Read–Eval–Print Loop).
int	shell_repl_loop(t_shell_context *sh_ctx)
{
	char	*line;

	while (1)
	{
		line = prompt_listener(MAIN_PROMPT);
		// ctrl-c pressed at prompt: handler sets g_singal_value
		if (g_latest_signal_status == SIGINT)
        {
			sh_ctx->last_status = 130;
            /* readline may return NULL or "" depending on behavior */
            if(line)
                track_alloc(line, ALLOC_PROMPT,sh_ctx);
            shell_clear_iteration(sh_ctx);
            continue;
        }
        /* Ctrl-D / EOF */  
		if (!line)
			shell_exit(sh_ctx,sh_ctx->last_status);
		/*norml line */
		track_alloc(line, ALLOC_PROMPT, sh_ctx);
		if (line[0] != '\0')
		{
			add_history(line);
			sh_ctx->last_status = prompt_execution(line, sh_ctx);
		}
		shell_clear_iteration(sh_ctx);
	}
    return (sh_ctx->last_status);
}
int	prompt_execution(char *line, t_shell_context *sh_ctx)
{
	t_ast	*ast;
    t_list *token_list;
	int		status;

    status = lexer(line, &token_list, sh_ctx);

    if(status == EXIT_SUCCESS && token_list)
    {
        status = parser(token_list, &ast, sh_ctx);
        if(status == EXIT_SUCCESS && ast)
        {
            status = collect_all_heredocs(ast, sh_ctx);
            if (status == EXIT_SUCCESS)
                status = execute(ast, RUN_IN_SHELL, sh_ctx);
        }
    }
	return (status);
}

/*non interactive input handler for testing purpose*/
// TODO
char	*non_interactive_input(void)
{
	return (NULL);
}
char	*prompt_listener(t_prompt_mode mode)
{
	char	*line;

	line = NULL;
	// 每次开始 read 输入之前 reset signal
	g_latest_signal_status = 0;
    errno=0;
	if (!isatty(STDIN_FILENO))
	{
		// rl_replace_line("", 0);
		// rl_on_new_line();
		if (mode == MAIN_PROMPT)
		{
			set_signal_in_main_prompt_mode();
			line = readline("minishell >");
			set_signal_in_exe_main_process();
		}
		else if (mode == HEREDOC_PROMPT)
		{
			set_signal_in_heredoc_prompt_mode();
			line = readline("> ");
			set_signal_in_exe_main_process();
		}
	}
	else
		line = non_interactive_input();
	return (line);
}
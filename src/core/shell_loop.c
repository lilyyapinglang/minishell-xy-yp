/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:57:38 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 20:36:30 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"
#include "get_next_line.h"
#include "ms_readline.h"
#include "parser.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*non interactive input handler for testing purpose*/
char	*non_interactive_input(void)
{
	char	*line;
	char	*trimmed;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	trimmed = ft_strtrim(line, "\n");
	free(line);
	return (trimmed);
}
// TODO
// void	track_alloc(char *line, t_tracking_scope scope,
// 		t_shell_context *sh_ctx)
// {
// 	(void)line;
// 	(void)scope;
// 	(void)sh_ctx;
// }

/*
before starting to read, reset signal each time
*/
char	*prompt_listener(t_prompt_mode mode)
{
	char	*user_input;

	user_input = NULL;
	g_latest_signal_status = 0;
	errno = 0;
	if (isatty(STDIN_FILENO))
	{
		if (mode == MAIN_PROMPT)
		{
			set_signal_in_main_prompt_mode();
			user_input = readline("minishell > ");
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
// ctrl-c pressed at prompt: handler sets g_singal_value
/* readline may return NULL or "" depending on behavior */
/* Ctrl-D / EOF , user_input == NULL */
/*normal line */
int	shell_repl_loop(t_shell_context *sh_ctx)
{
	char	*user_intput;

	while (1)
	{
		user_intput = prompt_listener(MAIN_PROMPT);
		if (g_latest_signal_status == SIGINT)
		{
			sh_ctx->last_status = 130;
			if (user_intput)
				track_alloc(user_intput, ALLOC_PROMPT, sh_ctx);
			shell_clear_iteration(sh_ctx);
			continue ;
		}
		if (!user_intput)
			shell_exit(sh_ctx, sh_ctx->last_status);
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

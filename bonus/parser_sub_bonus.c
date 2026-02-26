/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sub_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:45:41 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 21:10:50 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parse_error.h"   // only it uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_ast	*parse_and_or(t_list **token, t_shell_context *sh)
{
	t_ast			*left;
	t_ast			*right;
	t_token_type	op;

	left = parse_pipeline(token, sh);
	if (sh->parsing_error)
		return (NULL);
	while (*token && is_and_or(peek_list(*token)))
	{
		op = peek_list(*token);
		advance_list(token);
		right = parse_pipeline(token, sh);
		if (sh->parsing_error)
			return (NULL);
		left = create_ast_and_or(left, op, right, sh);
		if (sh->parsing_error)
			return (NULL);
	}
	return (left);
}

t_ast	*parse_pipeline(t_list **token, t_shell_context *sh)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_redir(token, sh);
	if (sh->parsing_error)
		return (NULL);
	while (*token && peek_list(*token) == TOKEN_PIPE)
	{
		advance_list(token);
		right = parse_redir(token, sh);
		if (sh->parsing_error)
			return (NULL);
		left = create_ast_pipeline(left, right, sh);
		if (sh->parsing_error)
			return (NULL);
	}
	return (left);
}

t_ast	*parse_subshell(t_list **token, t_shell_context *sh)
{
	t_ast	*content;

	if (match_list(token, TOKEN_LPAREN))
	{
		content = parse_and_or(token, sh);
		if (sh->parsing_error)
			return (NULL);
		if (match_list(token, TOKEN_RPAREN))
		{
			if (*token && is_word(peek_list(*token)))
				return (set_syntax_error(tk_value(*token), sh));
			return (create_ast_subshell(content, sh));
		}
		if (!*token || peek_list(*token) == TOKEN_EOF)
			return (set_syntax_error("\\n", sh));
		return (set_syntax_error(tk_value(*token), sh));
	}
	if (*token && peek_list(*token) == TOKEN_RPAREN)
		return (set_syntax_error(")", sh));
	return (parse_command(token, sh));
}

t_ast	*parse_command(t_list **token, t_shell_context *sh)
{
	t_list	*scan;
	char	**argv;
	int		argc;
	int		i;

	scan = *token;
	argc = 0;
	while (scan && tk_type(scan) == TOKEN_WORD)
	{
		argc++;
		scan = scan->next;
	}
	if (argc == 0)
		return (NULL);
	argv = calloc_s(argc + 1, sizeof(char *), ALLOC_PROMPT, sh);
	i = 0;
	while (i < argc)
	{
		argv[i] = tk_value(*token);
		advance_list(token);
		i++;
	}
	argv[argc] = NULL;
	return (create_ast_command(argv, sh));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:45:41 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:16:04 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"   // only it uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_ast	*parse_and_or(t_list **token, t_shell_context *sh)
{
	return (parse_pipeline(token, sh));
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

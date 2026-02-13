/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:58 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/13 16:13:15 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"   // defines t_shell_context (parsing_error)
#include "parse.h"       // AST/token types (may already be in minishell.h)
#include "parse_error.h" // set_syntax_error, report_syntax_error
#include "parser.h"      // (if you create it) parse_and_or/peek_list etc

// #include "minishellparse.h"
/*
int	parser(t_list *token_list, t_ast **ast, t_shell_context *sh)
{
	*ast = parse_and_or(&token_list, sh);
	if (token_list)
		set_syntax_error(tk_value(token_list), sh);
	if (sh->parsing_error)
		return (report_syntax_error(sh));
	return (EXIT_SUCCESS);
}
*/

int	parser(t_list *token_list, t_ast **ast, t_shell_context *sh)
{
	const char	*tok = tk_value(token_list);

	*ast = parse_and_or(&token_list, sh);
	if (!sh->parsing_error && token_list && peek_list(token_list) != TOKEN_EOF)
	{
		if (!tok)
			tok = tk_type_to_string(tk_type(token_list));
		set_syntax_error((char *)tok, sh);
	}
	if (sh->parsing_error)
		return (report_syntax_error(sh));
	return (EXIT_SUCCESS);
}

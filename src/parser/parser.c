/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:58 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 18:34:39 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"
/*
int parser(t_list *token_list, t_ast **ast, t_shell_context *sh)
{
	*ast = parse_and_or(&token_list, sh);
	if (token_list)
		set_syntax_error(tk_value(token_list), sh);
	if (sh->parsing_error)
		return (report_syntax_error(sh));
	return (EXIT_SUCCESS);
}
*/

int parser(t_list *token_list, t_ast **ast, t_shell_context *sh)
{
    *ast = parse_and_or(&token_list, sh);

    if (!sh->parsing_error
        && token_list
        && peek_list(token_list) != TOKEN_EOF)
    {
        const char *tok = tk_value(token_list);
        if (!tok)
            tok = tk_type_to_string(tk_type(token_list));
        set_syntax_error((char *)tok, sh);
    }

    if (sh->parsing_error)
        return (report_syntax_error(sh));
    return (EXIT_SUCCESS);
}


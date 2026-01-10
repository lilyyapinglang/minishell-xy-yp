/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:16:00 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/10 14:53:50 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parse_error.h"   // only if it uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

void init_ast_node(t_ast **node, t_ast_type type, t_shell_context *sh)
{
    *node = calloc_s(1, sizeof(t_ast), ALLOC_PROMPT, sh);
    (*node)->type = type;
}

char *tk_type_to_string(t_token_type type)
{
    if (type == TOKEN_PIPE)
        return ("|");
    else if (type == TOKEN_AND)
        return ("&&");
    else if (type == TOKEN_OR)
        return ("||");
    else if (type == TOKEN_LPAREN)
        return ("(");
    else if (type == TOKEN_RPAREN)
        return (")");
    else if (type == TOKEN_WORD)
        return ("word");
    else if (type == REDIR_INPUT)
        return ("<");
    else if (type == REDIR_OUTPUT)
        return (">");
    else if (type == REDIR_APPEND)
        return (">>");
    else if (type == REDIR_HEREDOC)
        return ("<<");
    else if (type == TOKEN_EOF)
        return ("\\n");
    else
        return ("[unknown token]");
}
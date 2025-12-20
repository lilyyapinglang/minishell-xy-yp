/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:57:53 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:28:42 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"
#include "../include/minishell.h"

static int	is_redir(t_token_type t)
{
	if (t == REDIR_INPUT || t == REDIR_OUTPUT
		|| t == REDIR_APPEND || t == REDIR_HEREDOC)
		return (1);
	return (0);
}

static int	is_binary_op(t_token_type t)
{
	if (t == TOKEN_PIPE || t == TOKEN_AND || t == TOKEN_OR)
		return (1);
	return (0);
}

static int	is_wrong_start(t_token_type t)
{
	if (t == TOKEN_PIPE || t == TOKEN_AND
		|| t == TOKEN_OR || t == TOKEN_RPAREN)
		return (1);
	return (0);
}

static int	is_wrong_after_op(t_token_type t)
{
	if (t == TOKEN_PIPE || t == TOKEN_AND || t == TOKEN_OR
		|| t == TOKEN_RPAREN || t == TOKEN_EOF)
		return (1);
	return (0);
}

static int	syntax_error_token(t_tokenlist *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return (syntax_error("newline"), 0);
	if (tok->value && tok->value[0])
		return (syntax_error(tok->value), 0);
	return (syntax_error("syntax error"), 0);
}



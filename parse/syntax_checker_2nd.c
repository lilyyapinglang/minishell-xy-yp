/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_2nd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:02:45 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:28:54 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"
#include "../include/minishell.h"

int	check_redirect(t_tokenlist *cur)
{
	if (!cur->next)
		return (syntax_error_token(NULL));
	if (cur->next->type != TOKEN_WORD)
		return (syntax_error_token(cur->next));
	return (1);
}

int	check_binary_op(t_tokenlist *cur)
{
	if (!cur->next)
		return (syntax_error_token(NULL));
	if (is_wrong_after_op(cur->next->type))
		return (syntax_error_token(cur->next));
	return (1);
}

int	check_lparen(t_tokenlist *cur)
{
	t_token_type	nxt;

	if (!cur->next)
		return (syntax_error_token(NULL));
	nxt = cur->next->type;
	if (nxt == TOKEN_RPAREN || nxt == TOKEN_PIPE
		|| nxt == TOKEN_AND || nxt == TOKEN_OR || nxt == TOKEN_EOF)
		return (syntax_error_token(cur->next));
	return (1);
}

int	check_rparen_follow(t_tokenlist *cur)
{
	if (!cur->next)
		return (1);
	if (cur->next->type == TOKEN_WORD || cur->next->type == TOKEN_LPAREN)
		return (syntax_error_token(cur->next));
	if (is_redir(cur->next->type))
		return (syntax_error_token(cur->next));
	return (1);
}

int	check_rparen(t_tokenlist *prev, t_tokenlist *cur)
{
	if (!prev)
		return (syntax_error_token(cur));
	if (prev->type == TOKEN_LPAREN || is_binary_op(prev->type) || is_redir(prev->type))
		return (syntax_error_token(cur));
	return (check_rparen_follow(cur));
}
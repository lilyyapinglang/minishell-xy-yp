/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_3rd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:27:41 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:28:57 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"
#include "../include/minishell.h"

int	check_end_token(t_tokenlist *last, int parens)
{
	if (parens != 0)
		return (syntax_error(")"), 0);
	if (!last)
		return (1);
	if (is_binary_op(last->type) || is_redir(last->type) || last->type == TOKEN_LPAREN)
		return (syntax_error_token(NULL));
	return (1);
}

int	check_one_token(t_tokenlist *prev, t_tokenlist *cur, int *parens)
{
	if (cur->type == TOKEN_INVALID)
		return (syntax_error_token(cur));
	if (cur->type == TOKEN_LPAREN)
	{
		(*parens)++;
		if (!check_lparen(cur))
			return (0);
	}
	if (cur->type == TOKEN_RPAREN)
	{
		(*parens)--;
		if (*parens < 0)
			return (syntax_error_token(cur));
		if (!check_rparen(prev, cur))
			return (0);
	}
	if (is_redir(cur->type) && !check_redirect(cur))
		return (0);
	if (is_binary_op(cur->type) && !check_binary_op(cur))
		return (0);
	return (1);
}

int	parser_check_syntax(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	t_tokenlist	*prev;
	int			parens;

	if (!tokens)
		return (1);
	cur = tokens;
	prev = NULL;
	parens = 0;
	if (is_wrong_start(cur->type))
		return (syntax_error_token(cur));
	while (cur)
	{
		if (!check_one_token(prev, cur, &parens))
			return (0);
		prev = cur;
		cur = cur->next;
	}
	return (check_end_token(prev, parens));
}
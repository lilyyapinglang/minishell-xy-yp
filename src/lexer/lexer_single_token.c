/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_single_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:45:18 by xueyan_wang       #+#    #+#             */
/*   Updated: 2026/02/26 20:36:58 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parse.h"
#include "error.h"
#include "safefunctions.h"

t_token_type	scan_next_token(char *input, size_t *len, t_shell_context *sh)
{
	if (*input == '&' || *input == '|')
		return (token_and_or_pipe(input, *input, len, sh));
	else if (*input == '<' || *input == '>')
		return (token_redir(input, *input, len, sh));
	return (token_word(input, len, sh));
}

t_token_type	token_and_or_pipe(const char *input, char c, size_t *len,
		t_shell_context *sh)
{
	size_t	n;

	n = 0;
	while (input[n] == c)
		n++;
	*len = n;
	if (c == '&')
	{
		set_syntax_error("&", sh);
		return (TOKEN_INVALID);
	}
	if (n == 1)
		return (TOKEN_PIPE);
	set_syntax_error("||", sh);
	return (TOKEN_INVALID);
	if (n == 1)
		return (TOKEN_PIPE);
	set_syntax_error("||", sh);
	return (TOKEN_INVALID);
}

t_token_type	token_redir(char *input, char c, size_t *len,
		t_shell_context *sh)
{
	while (input[*len] == c)
		*len += 1;
	if (c == '<' && *len == 1)
		return (REDIR_INPUT);
	if (c == '<' && *len == 2)
		return (REDIR_HEREDOC);
	if (c == '>' && *len == 1)
		return (REDIR_OUTPUT);
	if (c == '>' && *len == 2)
		return (REDIR_APPEND);
	set_syntax_error(input, sh);
	return (TOKEN_INVALID);
}

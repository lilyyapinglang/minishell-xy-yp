/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_single_token_word.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyan_wang <xueyan_wang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:48:56 by xueyan_wang       #+#    #+#             */
/*   Updated: 2026/02/10 20:50:14 by xueyan_wang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parse.h"
#include "parse_error.h"
#include "safefunctions.h"

static void	update_quote_state(char cur, bool *in_quote, char *quote_c)
{
	if (!*in_quote)
	{
		*quote_c = cur;
		*in_quote = true;
	}
	else if (cur == *quote_c)
		*in_quote = false;
}

static bool	should_stop_word(char cur, bool in_quote)
{
	if (in_quote)
		return (false);
	if (ft_istoken(cur) || ft_isspace(cur))
		return (true);
	return (false);
}

t_token_type	token_word(char *input, size_t *len, t_shell_context *sh)
{
	char	quote_c;
	bool	in_quote;

	in_quote = false;
	quote_c = 0;
	while (input[*len])
	{
		if (input[*len] == '\"' || input[*len] == '\'')
			update_quote_state(input[*len], &in_quote, &quote_c);
		else if (should_stop_word(input[*len], in_quote))
			break ;
		*len += 1;
	}
	if (in_quote)
	{
		set_syntax_error("unclosed quote", sh);
		return (TOKEN_INVALID);
	}
	return (TOKEN_WORD);
}

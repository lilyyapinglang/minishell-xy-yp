/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utile_all.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 21:41:25 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/03 15:47:11 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"

int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token_type    get_operator_type(char *str, int *len)
{
    if (str[0] == '<' && str[1] == '<')
    {
        *len = 2;
        return (TOKEN_DLESS);
    }
    if (str[0] == '>' && str[1] == '>')
    {
        *len = 2;
        return (TOKEN_DGREAT);
    }

    if (str[0] == '|')
        return (*len = 1, TOKEN_PIPE);
    if (str[0] == '<')
        return (*len = 1, TOKEN_LESS);
    if (str[0] == '>')
        return (*len = 1, TOKEN_GREAT);

    return (*len = 0, TOKEN_EOF);
}

void	print_quote_error(t_quote_state quote)
{
	if (quote == QUOTE_SINGLE)
		write(2, "minishell: unexpected EOF while looking "
			"for matching `''\n", 58);
	else if (quote == QUOTE_DOUBLE)
		write(2, "minishell: unexpected EOF while looking "
			"for matching `\"'\n", 58);
}

int	handle_quotes(t_tokenize *lex, char quote_char)
{
	if (lex->quote == QUOTE_NONE)
	{
		if (quote_char == '\'')
			lex->quote = QUOTE_SINGLE;
		else if (quote_char == '"')
			lex->quote = QUOTE_DOUBLE;
		return (1);
	}
	else if ((lex->quote == QUOTE_SINGLE && quote_char == '\'')
		|| (lex->quote == QUOTE_DOUBLE && quote_char == '"'))
	{
		lex->quote = QUOTE_NONE;
		return (1);
	}
	return (0);
}

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	return (ptr);
}
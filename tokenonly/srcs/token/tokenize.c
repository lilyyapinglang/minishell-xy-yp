/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 21:35:29 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/03 15:52:50 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"

static int handle_operator(t_tokenize *lex)
{
	t_token_type type;
	int len;
	char *value;
	t_tokenlist *token;

	type = get_operator_type(&lex->input[lex->pos], &len);
	value = ft_substr(lex->input, lex->pos, len);
	if (!value)
		return (0);
	token = token_create(type, value);
	free(value);
	if (!token)
		return (0);
	token_add(&lex->tokens, token);
	lex->pos += len;
	return (1);
}

static int handle_word(t_tokenize *lex)
{
	char *word;
	t_tokenlist *token;

	word = extract_word(lex);
	if (!word)
		return (0);
	if (*word)
	{
		token = token_create(TOKEN_WORD, word);
		if (!token)
		{
			free(word);
			return (0);
		}
		token_add(&lex->tokens, token);
	}
	free(word);
	return (1);
}

static int tokenize_loop(t_tokenize *lex)
{
	while (lex->input[lex->pos])
	{
		while (lex->input[lex->pos] && ft_isspace(lex->input[lex->pos]))
			lex->pos++;
		if (!lex->input[lex->pos])
			break;

		if (lex->quote == QUOTE_NONE && is_operator_char(lex->input[lex->pos]))
		{
			if (!handle_operator(lex))
				return (0);
		}
		else
		{
			if (!handle_word(lex))
				return (0);
		}
	}
	return (1);
}

t_tokenlist *lexer_tokenize(char *input)
{
	t_tokenize lex;

	if (!input)
		return (NULL);
	lex.input = input;
	lex.pos = 0;
	lex.quote = QUOTE_NONE;
	lex.tokens = NULL;
	if (!tokenize_loop(&lex))
	{
		ft_putstr_fd("minishell: Fatal Error during tokenization\n", 2);
		token_list_free(lex.tokens);
		return (NULL);
	}
	if (lex.quote != QUOTE_NONE)
	{
		print_quote_error(lex.quote);
		token_list_free(lex.tokens);
		return (NULL);
	}
	return (lex.tokens);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 21:43:43 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/03 15:47:02 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	meet_break(t_tokenize *lex, char c)
{
	if (lex->quote != QUOTE_NONE)
		return (0);
	if (ft_isspace(c))
		return (1);
	if (is_operator_char(c))
		return (1);
	return (0);
}

static int	realloc_and_append(char **word, size_t *size, size_t *cap, char c)
{
	char	*new_word;

	if (*size >= *cap - 1)
	{
		*cap *= 2;
		new_word = realloc(*word, *cap);
		if (!new_word)
		{
			free(*word);
			*word = NULL;
			return (0);
		}
		*word = new_word;
	}
	(*word)[*size] = c;
	(*size)++;
	return (1);
}

static char	*get_raw_word(t_tokenize *lex)
{
	char	*word;
	size_t	size;
	size_t	capacity;
	char	current_char;

	capacity = 356;
	word = malloc(capacity);
	if (!word)
		return (NULL);
	size = 0;
	while (lex->input[lex->pos])
	{
		current_char = lex->input[lex->pos];

		if (meet_break(lex, current_char))
			break ;
		if (current_char == '\'' || current_char == '"')
			handle_quotes(lex, current_char);
		if (!realloc_and_append(&word, &size, &capacity, current_char))
			return (NULL);
		lex->pos++;
	}
	word[size] = '\0';
	return (word);
}

char	*extract_word(t_tokenize *lex)
{
	char	*raw_word;
	char	*result;

	raw_word = get_raw_word(lex);

	if (!raw_word)
		return (NULL);
	result = ft_strdup(raw_word);
	free(raw_word);
	return (result);
} 

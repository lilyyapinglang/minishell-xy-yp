/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 21:45:08 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/02 22:39:34 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"

t_tokenlist *token_create(t_token_type type, char *value)
{
	t_tokenlist *token;

	token = safe_malloc(sizeof(t_tokenlist));
	if (!token)
		return (NULL);

	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

void token_add(t_tokenlist **list, t_tokenlist *new)
{
	t_tokenlist *last;

	if (!list || !new)
		return;

	if (!*list)
	{
		*list = new;
		return;
	}

	last = *list;
	while (last->next)
		last = last->next;

	last->next = new;
}

void token_list_free(t_tokenlist *tokens)
{
	t_tokenlist *tmp;

	while (tokens)
	{
		tmp = tokens->next;

		if (tokens->value)
			free(tokens->value);
		free(tokens);

		tokens = tmp;
	}
}
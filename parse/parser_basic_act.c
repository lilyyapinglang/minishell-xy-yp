/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_basic_act.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:09:56 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 19:13:25 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"

/* detect what next */
t_token_type	peek(t_parser *p)
{
	if (p == NULL)
		return (TOKEN_EOF);
	if (p->current == NULL)
		return (TOKEN_EOF);
	return (p->current->type);
}

/* eat */
void	advance(t_parser *p)
{
	if (p == NULL)
		return ;
	if (p->current != NULL)
		p->current = p->current->next;
}

/* detect and eat */
int	match(t_parser *p, t_token_type type)
{
	if (peek(p) == type)
	{
		advance(p);
		return (1);
	}
	return (0);
}

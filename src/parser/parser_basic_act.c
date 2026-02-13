/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_basic_act.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyan_wang <xueyan_wang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:28:51 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/10 20:06:46 by xueyan_wang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parse_error.h"   // only if uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_token_type	peek_list(t_list *tok)
{
	if (!tok)
		return (TOKEN_EOF);
	return (tk_type(tok));
}

void	advance_list(t_list **tok)
{
	if (tok && *tok)
		*tok = (*tok)->next;
}

int	match_list(t_list **tok, t_token_type type)
{
	if (peek_list(*tok) == type)
	{
		advance_list(tok);
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_basic_act.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:28:51 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 16:30:53 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

t_token_type peek_list(t_list *tok)
{
    if (!tok)
        return (TOKEN_EOF);
    return (tk_type(tok));
}

void advance_list(t_list **tok)
{
    if (tok && *tok)
        *tok = (*tok)->next;
}

int match_list(t_list **tok, t_token_type type)
{
    if (peek_list(*tok) == type)
    {
        advance_list(tok);
        return (1);
    }
    return (0);
}

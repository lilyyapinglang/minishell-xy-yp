/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tk_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:17:19 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 16:30:31 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

t_token_type	tk_type(t_list *token)
{
	return (((t_token *)token->content)->type);
}

char	*tk_value(t_list *token)
{
	if (token == NULL)
		return (NULL);
	return (((t_token *)token->content)->value);
}

int is_and_or(t_token_type t)
{
    return (t == TOKEN_AND || t == TOKEN_OR);
}

int is_word(t_token_type t)
{
    return (t == TOKEN_WORD);
}

int is_redir(t_token_type t)
{
    return (t == REDIR_INPUT
        || t == REDIR_OUTPUT
        || t == REDIR_APPEND
        || t == REDIR_HEREDOC);
}
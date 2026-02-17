/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:40:12 by xueyan_wang       #+#    #+#             */
/*   Updated: 2026/02/13 16:12:27 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h" // ft_substr (adjust path to your real libft header)
#include "lexer.h"          // (you’ll create this)
#include "parse.h"          // t_token, t_token_type
#include "parse_error.h"
#include "safefunctions.h" // calloc_s, s_alloc, lst_add_back_s
#include "tracking.h"      // PROMPT_S and t_tracking_scope
#include <stdlib.h>        // EXIT_SUCCESS

/*
int	lexer(char *input, t_list **token_list, t_shell_context *sh)
{
	t_token			*token;
	t_token_type	type;
	size_t			i;
	size_t			len;

	i = 0;
	*token_list = NULL;
	while (input && input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (!input[i])
			return (EXIT_SUCCESS);
		len = 0;
		type = scan_next_token(input + i, &len, sh);
		if (type == TK_ERROR)
			return (report_syntax_error(sh));
		token = create_token(type, input + i, len, sh);
		add_back_list_s(token, token_list, PROMPT, sh);
		i += len;
	}
	return (EXIT_SUCCESS);
}
*/
int	lexer(char *input, t_list **token_list, t_shell_context *sh)
{
	t_token			*token;
	t_token_type	type;
	size_t			i;
	size_t			len;

	i = 0;
	*token_list = NULL;
	while (input[i])
	{
		len = 0;
		if (!ft_isspace(input[i]))
		{
			type = scan_next_token(input + i, &len, sh);
			if (type == TOKEN_INVALID)
				return (report_syntax_error(sh));
			token = create_token(type, input + i, len, sh);
			lst_add_back_s(token, token_list, ALLOC_PROMPT, sh);
		}
		else
			len++;
		i += len;
	}
	return (EXIT_SUCCESS);
}

t_token	*create_token(t_token_type type, char *value, size_t len,
		t_shell_context *sh)
{
	t_token	*token;

	token = calloc_s(1, sizeof(t_token), ALLOC_PROMPT, sh);
	token->type = type;
	token->value = s_alloc(ft_substr(value, 0, len), ALLOC_PROMPT, sh);
	return (token);
}

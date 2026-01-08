/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:40:12 by xueyan_wang       #+#    #+#             */
/*   Updated: 2025/12/30 18:04:30 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"
/*
int lexer(char *input, t_list **token_list, t_shell_context *sh)
{
	t_token *token;
	t_token_type type;
	size_t i;
	size_t len;

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
			lst_add_back_s(token, token_list, PROMPT_S, sh);
		}
		else
			len++;
		i += len;
	}
	return (EXIT_SUCCESS);
}
t_token *create_token(t_token_type type, char *value, size_t len, t_shell_context *sh)
{
	t_token *token;

	token = calloc_s(1, sizeof(t_token), PROMPT_S, sh);
	token->type = type;
	token->value = s_alloc(ft_substr(value, 0, len), PROMPT_S, sh);
	return (token);
}

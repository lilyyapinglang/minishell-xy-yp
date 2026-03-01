/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc_delimiter.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 16:55:30 by lilypad           #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	hanlde_quote_none(char *ptr, t_quote_state *mode, bool *is_quoted,
		char **result)
{
	if (*ptr == '\'')
	{
		*mode = QUOTE_SINGLE;
		if (*is_quoted == false)
			*is_quoted = true;
	}
	else if (*ptr == '"')
	{
		*mode = QUOTE_DOUBLE;
		if (*is_quoted == false)
			*is_quoted = true;
	}
	else
		*(*result)++ = *ptr;
}

char	*update_result_loc(const char *raw, char *result, t_quote_state *mode,
		bool *is_quoted)
{
	char	*ptr;

	ptr = (char *)raw;
	while (*ptr)
	{
		if (*mode == QUOTE_NONE)
			hanlde_quote_none(ptr, mode, is_quoted, &result);
		else if (*mode == QUOTE_SINGLE)
		{
			if (*ptr == '\'')
				*mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		else if (*mode == QUOTE_DOUBLE)
		{
			if (*ptr == '"')
				*mode = QUOTE_NONE;
			else
				*result++ = *ptr;
		}
		ptr++;
	}
	return (result);
}

char	*heredoc_delimiter_strip(const char *raw, bool *is_quoted,
		t_shell_context *sh_ctx)
{
	t_quote_state	mode;
	char			*result;
	char			*result_ptr;

	(void)sh_ctx;
	if (!raw)
		return (NULL);
	if (is_quoted)
		*is_quoted = false;
	mode = QUOTE_NONE;
	result = malloc(ft_strlen(raw) + 1);
	if (!result)
		return (NULL);
	result_ptr = result;
	result = update_result_loc(raw, result, &mode, is_quoted);
	if (mode != QUOTE_NONE)
	{
		free(result_ptr);
		print_msg("heredoc", raw, "syntax error (unclosed quote)\n");
		return (NULL);
	}
	*result = '\0';
	return (result_ptr);
}

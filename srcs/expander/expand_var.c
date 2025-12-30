/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:49 by fvastena          #+#    #+#             */
/*   Updated: 2025/12/30 13:54:03 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"
#include "../include/minishell.h"

/**
	@brief Expand variable in a string
*/
void expand_var(char *str, t_expander *exp, t_shell_context *sh)
{
	char *value;

	if (str[exp->i + 1] == '?')
		expand_last_status(exp, sh);
	else
	{
		value = expand_env_var(str, exp, sh);
		if (!value)
			return;
		else if (exp->context == NO_QUOTE)
			value = word_splitting(exp, value, sh);
		if (value)
			add_variable_to_buffer(value, exp, sh);
	}
}

void expand_last_status(t_expander *exp, t_shell_context *sh)
{
	char *last_exit_status;

	exp->i++;
	last_exit_status = s_alloc(ft_itoa(sh->last_status), PROMPT, sh);
	add_variable_to_buffer(last_exit_status, exp, sh);
}

/**
	@brief Expand `~` into the home path
*/
void expand_tilde(char *str, t_expander *exp, t_shell_context *sh)
{
	char *home_path;
	bool path;

	path = false;
	if (str[exp->i + 1] == '/' || (str[exp->i + 1] == '\0' && exp->buf_i == 0))
		path = true;
	home_path = value(env_var("HOME", sh->env));
	if (!home_path)
		home_path = sh->home;
	if (home_path && path)
		add_variable_to_buffer(home_path, exp, sh);
	else
		exp->buf[exp->buf_i++] = str[exp->i];
}

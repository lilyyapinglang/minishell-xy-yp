/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:08:17 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/25 15:51:52 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "env.h"
#include "expander.h"
#include "ms_error.h"
#include "parse.h"
#include "safefunctions.h"
#include "shell_context.h"

void	expand_last_status(t_expander *exp, t_shell_context *sh)
{
	char	*last_exit_status;

	exp->i++;
	last_exit_status = s_alloc(ft_itoa(sh->last_status), ALLOC_PROMPT, sh);
	add_variable_to_buffer(last_exit_status, exp, sh);
}
void	expand_var(char *str, t_expander *exp, t_shell_context *sh)
{
	char	*value;

	if (str[exp->i + 1] == '?')
		expand_last_status(exp, sh);
	else if (exp->context == NO_QUOTE
		&& (str[exp->i + 1] == '\"' || str[exp->i + 1] == '\''))//Xueyan add to handle $"..."
		return ;
	else
	{
		value = expand_env_var(str, exp, sh);
		if (!value)
			return ;
		else if (exp->context == NO_QUOTE)
			value = word_splitting(exp, value, sh);
		if (value)
			add_variable_to_buffer(value, exp, sh);
	}
}

void	expand_tilde(char *str, t_expander *exp, t_shell_context *sh)
{
	char	*home_path;
	bool	path;

	path = false;
	if (str[exp->i + 1] == '/' || (str[exp->i + 1] == '\0' && exp->buf_i == 0))
		path = true;
	// home_path = value(env_var("HOME", sh->env)); // lily changed
	// char					*env_get_value(t_list *env, const char *name);
	home_path = env_get_value(sh->env, "HOME");
	// if (!home_path)
	//	home_path = sh->home;
	if (home_path && path)
		add_variable_to_buffer(home_path, exp, sh);
	else
		exp->buf[exp->buf_i++] = str[exp->i];
}

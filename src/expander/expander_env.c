/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:09:02 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 19:09:04 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"


char *expand_env_var(char *str, t_expander *exp, t_shell_context *sh)
{
	char *name;
	char *env_value;

	name = get_valid_name(str, exp, sh);
	if (name == NULL)
	{
		exp->buf[exp->buf_i++] = str[exp->i];
		exp->buf[exp->buf_i] = '\0';
		return (NULL);
	}
	env_value = value(env_var(name, sh->env));
	exp->i += ft_strlen(name);
	return (env_value);
}

char *get_valid_name(char *str, t_expander *exp, t_shell_context *sh)
{
	int j;
	char *name;

	j = exp->i + 1;
	if (!ft_isalpha(str[j]) && str[j] != '_')
		return (NULL);
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	name = s_alloc(ft_substr(str, exp->i + 1, j - exp->i - 1), PROMPT, sh);
	return (name);
}

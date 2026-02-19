/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 20:50:22 by ylang             #+#    #+#             */
/*   Updated: 2026/02/19 18:49:39 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/parse.h"

/*
Environment variable names used by the utilities in the Shell
and Utilities volume of POSIX.1-2024 consist solely of uppercase letters,
digits, and the <underscore> ('_') from the characters defined
in Portable Character Set and do not begin with a digit.

Invalid exameples:
str = null or str = ""
str = 9 etc..
*/
int	is_valid_var_name(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (ft_isdigit((unsigned char)str[0]))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!(ft_isalnum((unsigned char)str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_options(char *arg, char *cmd, char *valid_options)
{
	int	j;

	j = 1;
	while (arg[j])
	{
		if (ft_strchr(valid_options, arg[j]))
			return (print_msg_n_return(2, cmd, arg, "invalid option"));
		j++;
	}
	return (0);
}

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd,
			"env") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

// cd, export , unset, exit
bool	is_stateful_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

t_builtin_func	lookup_builtin_func(const char *name)
{
	static const t_builtin_entry	table[] = {{"echo", BI_ECHO, builtin_echo},
			{"pwd", BI_PWD, builtin_pwd}, {"env", BI_ENV, builtin_env}, {"cd",
			BI_CD, builtin_cd}, {"export", BI_EXPORT, builtin_export}, {"unset",
			BI_UNSET, builtin_unset}, {"exit", BI_EXIT, builtin_exit}, {NULL,
			BI_NONE, NULL}};
	int								i;

	i = 0;
	if (!name)
		return (NULL);
	while (table[i].name)
	{
		if (ft_strcmp(name, table[i].name) == 0)
			return (table[i].func);
		i++;
	}
	return (NULL);
}

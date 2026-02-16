/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 20:50:22 by ylang             #+#    #+#             */
/*   Updated: 2026/02/16 18:05:15 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/parse.h"

// str = null or str = ""
// str =9 etc
int	is_valid_ident(const char *str)
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

int	check_valid_options(char *arg, char *cmd, char *valid_options)
{
	int	j;

	j = 1;
	while (arg[j])
	{
		if (ft_strchr(valid_options, arg[j]))
			return (print_msg_n_return(2, "export", arg, "invalid option"));
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

static t_builtin_func	lookup_builtin_func(const char *name)
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
// 这里理论上不应该发生（外面已经判断 is_builtin）
// 但为了鲁棒性，返回 1 或 0 都行；通常返回 1 更像“内部错误”
// 注意：builtin 函数自己负责打印错误并返回对应 status
// execute_builtin 不要 exit（除了 builtin_exit 自己 exit）

int	execute_builtin(t_ast_command *cmd, t_shell_context *ctx)
{
	t_builtin_func	func;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	func = lookup_builtin_func(cmd->args[0]);
	if (!func)
		return (1);
	return (func(cmd->args, ctx));
}

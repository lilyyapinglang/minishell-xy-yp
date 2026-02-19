/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:48:27 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 18:46:04 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	append_value(char *arg, char *plus_eq, char *equal_sign_loc,
		t_shell_context *sh_ctx)
{
	char	*name;
	char	*val;
	int		status;

	name = NULL;
	status = 0;
	name = ft_substr(arg, 0, (size_t)(plus_eq - arg));
	val = equal_sign_loc + 1;
	if (!name || !is_valid_var_name(name))
	{
		print_msg_n_return(1, "export", arg, "not a valid identifier");
		status = 1;
	}
	else if (env_append_value(sh_ctx, name, val, true) != 0)
		status = 1;
	free(name);
	return (status);
}

int	set_new_value(char *arg, char *equal_sign_loc, t_shell_context *sh_ctx)
{
	char	*name;
	char	*val;
	int		status;

	status = 0;
	name = ft_substr(arg, 0, (size_t)(equal_sign_loc - arg));
	val = equal_sign_loc + 1;
	if (!name || !is_valid_var_name(name))
	{
		print_msg_n_return(1, "export", arg, "not a valid identifier");
		status = 1;
	}
	else if (env_set_value(sh_ctx, name, val, true) != 0)
		status = 1;
	free(name);
	return (status);
}

int	no_value(char *arg, t_shell_context *sh_ctx)
{
	int	status;

	status = 0;
	if (!is_valid_var_name(arg))
		status = print_msg_n_return(1, "export", arg, "not a valid identifier");
	else if (env_mark_exported(sh_ctx, arg) != 0)
		status = 1;
	return (status);
}

char	*get_plus_loc(char *equal_sign_loc, char *arg)
{
	char	*plus_eq;

	plus_eq = NULL;
	if (equal_sign_loc && equal_sign_loc > arg && equal_sign_loc[-1] == '+')
		plus_eq = equal_sign_loc - 1;
	return (plus_eq);
}

// need to sort and print
// check each if --test=100
// check - valid options
/* Detect NAME+=VALUE safely:
		only when there is an '=' and the char right before '=' is '+' */
/* NAME+=VALUE */
/* value starts after '=' */
/* You may choose to print malloc-related error here */
/* NAME=VALUE */
/* NAME (export only) */

int	builtin_export(char **argv, t_shell_context *sh_ctx)
{
	int		i;
	int		status;
	char	*equal_sign_loc;
	char	*plus_eq;

	i = 1;
	status = 0;
	plus_eq = NULL;
	if (!argv[1])
		return (print_env_or_export(true, sh_ctx), 0);
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (is_valid_options(argv[i], argv[0], "fnp"));
		equal_sign_loc = ft_strchr(argv[i], '=');
		plus_eq = get_plus_loc(equal_sign_loc, argv[i]);
		if (plus_eq)
			status = append_value(argv[i], plus_eq, equal_sign_loc, sh_ctx);
		else if (equal_sign_loc)
			status = set_new_value(argv[i], equal_sign_loc, sh_ctx);
		else
			status = no_value(argv[i], sh_ctx);
		i++;
	}
	return (status);
}

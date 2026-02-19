/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:48:12 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 20:30:34 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// unset env
int	env_unset(t_shell_context *sh_ctx, const char *name)
{
	t_list		*env;
	t_env_var	*env_var;

	env = sh_ctx->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (ft_strcmp(env_var->name, name) == 0)
		{
			del_node_from_env(env, sh_ctx);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

int	builtin_unset(char **argv, t_shell_context *sh_ctx)
{
	int		i;
	int		status;
	char	*arg;

	i = 1;
	status = 0;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		arg = argv[i];
		if (arg[0] == '-')
			return (is_valid_options(arg, argv[0], "fvn"));
		if (is_valid_var_name(argv[i]))
			status = env_unset(sh_ctx, argv[i]);
		i++;
	}
	return (status);
}

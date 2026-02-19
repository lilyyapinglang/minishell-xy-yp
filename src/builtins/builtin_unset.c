/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:48:12 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 18:46:44 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

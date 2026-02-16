/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:48:12 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 17:54:05 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_unset(char **argv, t_shell_context *ctx)
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
			return (check_valid_options(arg, argv[0], "fvn"));
		if (is_valid_ident(argv[i]))
			status = env_unset(ctx, argv[i]);
		i++;
	}
	return (status);
}

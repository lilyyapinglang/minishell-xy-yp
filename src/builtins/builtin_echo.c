/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:46:46 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 17:56:38 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	output_each_arg(char **strs)
{
	while (*strs)
	{
		ft_putstr_fd(*strs, STDOUT_FILENO);
		if (*(strs + 1))
			ft_putchar_fd(' ', STDOUT_FILENO);
		strs++;
	}
}

// buildin, execute in child or pipeline
// echo -n hello world
// -n means don't add newline charactor at the output
// argv[0] = echo
// argv[1] = xxx ?
// write each char to standard output and add \n
// not last one, add space
// if from 1 -> n-1 no -n
// write each char to stardard ouput
// printf("with -n options \n");

int	builtin_echo(char **argv, t_shell_context *sh_ctx)
{
	char	**strs;
	int		i;

	(void)sh_ctx;
	if (!argv[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	if (!(ft_strncmp(argv[1], "-n", 2) == 0 && is_only_n(&argv[1][1])))
	{
		strs = &argv[1];
		output_each_arg(strs);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else
	{
		i = 1;
		while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0
			&& is_only_n(&argv[i][1]))
			i++;
		strs = &argv[i];
		output_each_arg(strs);
	}
	return (0);
}

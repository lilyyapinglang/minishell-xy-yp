/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:47:30 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 17:56:08 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_numeric(char *str)
{
	if (str[0] == '+' || str[0] == '-')
	{
		if (check_all_digit(&str[1]))
			return (1);
		else
			return (0);
	}
	else
		return (check_all_digit(str));
}

// exit, but exit which process ???
/* no args => exit with last status */
/* non-numeric => error, exit 255 is common in bash;
/* too many args => error, DO NOT exit (matches common shell behavior) */
int	builtin_exit(char **argv, t_shell_context *ctx)
{
	if (!argv[1])
		exit(ctx->last_status);
	if (!is_numeric(argv[1]))
	{
		print_msg_n_return(255, "exit", argv[1], "numeric argument required");
		return (2);
	}
	if (argv[2])
		return (print_msg_n_return(1, "exit", NULL, "too many arguments"));
	exit(ft_atoi(argv[1]));
}

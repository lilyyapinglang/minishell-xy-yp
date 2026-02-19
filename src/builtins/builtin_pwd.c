/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:46:51 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 18:46:34 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// print working directory
int	builtin_pwd(char **argv, t_shell_context *sh_ctx)
{
	char	*pwd;

	(void)argv;
	(void)sh_ctx;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (print_errno_n_return(1, "pwd", NULL, errno));
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:49:45 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 17:56:16 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	update_env_pwd(char *oldpwd, char *newpwd, t_shell_context *ctx)
{
	if (oldpwd)
		env_set_value(ctx, "OLDPWD", oldpwd, true);
	if (newpwd)
		env_set_value(ctx, "PWD", newpwd, true);
	free(oldpwd);
	free(newpwd);
}
// change directories
/*
	Uses getcwd() to track current directory
	Falls back to HOME when no args
	Uses chdir()
	Updates PWD and OLDPWD
	Prints errors like bash
	Returns success/failure code
*/
// PWD=/home/ylang/code/minishell-github
// OLDPWD=/home/ylang/code/minishell-github
// not neccessary to interrupt cd even failed

int	builtin_cd(char **argv, t_shell_context *ctx)
{
	const char	*target_dir;
	char		*oldpwd;
	char		*newpwd;
	int			cwd_status;

	if (!argv[1])
	{
		target_dir = env_get_value(ctx->env, "HOME");
		if (!target_dir)
			return (print_msg_n_return(1, "cd", NULL, "HOME not set"));
	}
	else
		target_dir = argv[1];
	oldpwd = getcwd(NULL, 0);
	cwd_status = chdir(target_dir);
	if (cwd_status != 0)
	{
		free(oldpwd);
		return (print_errno_n_return(1, "cd", target_dir, errno));
	}
	newpwd = getcwd(NULL, 0);
	update_env_pwd(oldpwd, newpwd, ctx);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:46:51 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/16 17:54:57 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// print working directory
int	builtin_pwd(char **argv, t_shell_context *ctx)
{
	char	*pwd;

	(void)argv;
	(void)ctx;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (print_errno_n_return(1, "pwd", NULL, errno));
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

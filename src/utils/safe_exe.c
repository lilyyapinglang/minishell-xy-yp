/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_exe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:16:40 by ylang             #+#    #+#             */
/*   Updated: 2026/03/01 11:43:16 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/error.h"

int	open_s(const char *pathname, int flags, mode_t mode,
		t_shell_context *sh_ctx)
{
	int	ret;

	ret = open(pathname, flags, mode);
	if (ret == -1)
		syscall_error(pathname, errno, sh_ctx);
	return (ret);
}

int	close_s(int fd, t_shell_context *sh_ctx)
{
	int	ret;

	ret = close(fd);
	if (ret == -1)
		syscall_error("close", errno, sh_ctx);
	return (ret);
}

int	dup2_s(int oldfd, int newfd, t_shell_context *sh_ctx)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	if (ret == -1)
		syscall_error("dup2", errno, sh_ctx);
	return (ret);
}

int	stat_s(const char *restrict path, struct stat *restrict buf,
		t_shell_context *sh_ctx)
{
	int	ret;

	ret = stat(path, buf);
	if (ret == -1)
		syscall_error("stat", errno, sh_ctx);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_exe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:16:40 by ylang             #+#    #+#             */
/*   Updated: 2026/02/13 21:18:10 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	dup2_s(int fd1, int fd2, t_shell_context *sh_ctx)
{
	(void)fd1;
	(void)fd2;
	(void)sh_ctx;
	// TODO: implement
}

void	close_s(int fd, t_shell_context *sh_ctx)
{
	(void)fd;
	(void)sh_ctx;
	// TODO: implement
}

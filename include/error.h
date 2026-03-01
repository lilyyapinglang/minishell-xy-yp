/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:09 by xuewang           #+#    #+#             */
/*   Updated: 2026/03/01 00:00:00 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "shell_context.h"
# include <sys/types.h>

/* ── Low-level write ── */
ssize_t	ft_write_fd(const char *s, int fd);

/* ── Parse / syntax errors ── */
void	*set_syntax_error(char *unexpected_token, t_shell_context *sh);
int		report_syntax_error(t_shell_context *sh);
int		report_error(char *context, char *element,
			char *description, t_shell_context *sh);

/* ── Runtime errors ── */
void	syscall_error(const char *context, int errnum, t_shell_context *sh);
void	error(const char *context, char *description,
			int exit_status, t_shell_context *sh);

/* ── Shell exit ── */
void	quit_shell(int exit_status, t_shell_context *sh);

#endif

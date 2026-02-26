/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:17 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:34:02 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_ERROR_H
# define PARSE_ERROR_H

# include "shell_context.h"
# include <sys/types.h>

/* error */
ssize_t	ft_write_fd(const char *s, int fd);
void	*set_syntax_error(char *unexpected_token, t_shell_context *sh);
int		report_syntax_error(t_shell_context *sh);
int		report_error(char *context, char *element,
			char *description, t_shell_context *sh);
void	syscall_error(const char *context, int errnum, t_shell_context *sh);

/* quit_shell.c */
void	quit_shell(int exit_status, t_shell_context *sh);
void	error(const char *context, char *description,
			int exit_status, t_shell_context *sh);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:08:42 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/10 18:03:57 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "error.h"
#include "safefunctions.h"
#include "shell_context.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ssize_t	ft_write_fd(const char *s, int fd)
{
	if (!s)
		return (-1);
	return (write(fd, s, ft_strlen(s)));
}

void	*set_syntax_error(char *unexpected_token, t_shell_context *sh)
{
	if (!sh->parsing_error)
		sh->parsing_error = unexpected_token;
	return (NULL);
}

int	report_syntax_error(t_shell_context *sh)
{
	report_error("syntax error near unexpected token `", sh->parsing_error, "'",
		sh);
	sh->parsing_error = NULL;
	return (2);
}

int	report_error(char *context, char *element, char *description,
		t_shell_context *sh)
{
	write_s("minishell: ", STDERR_FILENO, sh);
	write_s(context, STDERR_FILENO, sh);
	if (element)
		write_s(element, STDERR_FILENO, sh);
	write_s(description, STDERR_FILENO, sh);
	write_s("\n", STDERR_FILENO, sh);
	return (EXIT_FAILURE);
}

void	syscall_error(const char *context, int errnum, t_shell_context *sh)
{
	error(context, strerror(errnum), EXIT_FAILURE, sh);
}
// ssize_t	write_s(const char *s, int fd, t_shell_context *sh)
// {
// 	ssize_t	ret;

// 	ret = ft_write_fd(s, fd);
// 	if (ret == -1)
// 		syscall_error("ft_write_fd", errno, sh);
// 	return (ret);
// }

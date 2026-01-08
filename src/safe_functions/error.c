/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:08:42 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 17:49:44 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

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
	report_error("syntax error near unexpected token `",
		sh->parsing_error, "'", sh);
	sh->parsing_error = NULL;
	return (2);
}

int	report_error(char *context, char *element, char *description, t_shell_context *sh)
{
	write_s("minishell: ", STDERR_FILENO, sh);
	write_s(context, STDERR_FILENO, sh);
	if (element)
		write_s(element, STDERR_FILENO, sh);
	write_s(description, STDERR_FILENO, sh);
	write_s("\n", STDERR_FILENO, sh);
	return (EXIT_FAILURE);
}

ssize_t	write_s(const char *s, int fd, t_shell_context *sh)
{
	ssize_t	ret;

	ret = ft_write_fd(s, fd);
	if (ret == -1)
		syscall_error("ft_write_fd", errno, sh);
	return (ret);
}

void	syscall_error(const char *context, int errnum, t_shell_context *sh)
{
	error(context, strerror(errnum), EXIT_FAILURE, sh);
}


/*
void	free_env_var(void *content)
{
	t_var	*var;

	var = (t_var *)content;
	free(var->name);
	free(var->value);
	free(var);
}
*/
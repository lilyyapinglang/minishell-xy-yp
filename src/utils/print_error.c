/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:17:32 by ylang             #+#    #+#             */
/*   Updated: 2026/02/19 23:26:51 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_raw(const char *str)
{
	if (str && *str)
		ft_putstr_fd((char *)str, STDERR_FILENO);
}

static void	put_segment(const char *str)
{
	if (str && *str)
	{
		put_raw(str);
		put_raw(": ");
	}
}

/*
** Bash-like format:
** minishell : [cmd: ] [arg:] message \n
*/
 void	print_error(const char *cmd, const char *arg, const char *msg)
{
	put_raw(SHELL_NAME);
	put_raw(": ");
	put_segment(cmd);
	put_segment(arg);
	if (!msg)
		msg = "";
	put_raw(msg);
	put_raw("\n");
}

/* ---------------- public: print only (optional) ---------------- */

void	print_errno(const char *cmd, const char *arg, int errnum)
{
	print_error(cmd, arg, strerror(errnum));
}

void	print_msg(const char *cmd, const char *arg, const char *msg)
{
	print_error(cmd, arg, msg);
}

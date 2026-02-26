/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_error.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:09 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:08:10 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_ERROR_H
# define MS_ERROR_H

# include <sys/types.h> // ssize_t

typedef struct s_shell_context t_shell_context;

ssize_t ft_write_fd(const char *s, int fd);

void   *set_syntax_error(char *unexpected_token, t_shell_context *sh);
int     report_syntax_error(t_shell_context *sh);

int     report_error(char *context, char *element,
                     char *description, t_shell_context *sh);

//ssize_t write_s(const char *s, int fd, t_shell_context *sh);
void    syscall_error(const char *context, int errnum, t_shell_context *sh);

void    error(const char *context, char *description,
              int exit_status, t_shell_context *sh);
void    quit_shell(int exit_status, t_shell_context *sh);

#endif
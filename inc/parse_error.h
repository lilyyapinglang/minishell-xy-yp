#ifndef PARSE_ERROR_H
# define PARSE_ERROR_H

#include "shell_context.h"
#include <sys/types.h>

/*error*/
ssize_t				ft_write_fd(const char *s, int fd);
void				*set_syntax_error(char *unexpected_token,
						t_shell_context *sh);
int					report_syntax_error(t_shell_context *sh);
int					report_error(char *context, char *element,
						char *description, t_shell_context *sh);
// ssize_t				write_s(const char *s, int fd, t_shell_context *sh);
void				syscall_error(const char *context, int errnum,
						t_shell_context *sh);

/*quiteshell.c*/
void				quit_shell(int exit_status, t_shell_context *sh);
void				error(const char *context, char *description,
						int exit_status, t_shell_context *sh);

#endif
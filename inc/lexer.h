#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "list.h"
# include "parse.h"

#include "shell_context.h"

/* main entrypoint */
int         lexer(char *input, t_list **token_list, t_shell_context *sh);

/* helpers used across lexer *.c */
int         ft_isspace(int c);
int         ft_istoken(int c);

t_token_type    scan_next_token(char *input, size_t *len, t_shell_context *sh);
t_token_type    token_subshell(char *input, size_t *len);
t_token_type    token_and_or_pipe(const char *input, char c, size_t *len, t_shell_context *sh);
t_token_type    token_redir(char *input, char c, size_t *len, t_shell_context *sh);
t_token_type    token_word(char *input, size_t *len, t_shell_context *sh);

t_token      *create_token(t_token_type type, char *value, size_t len, t_shell_context *sh);

#endif
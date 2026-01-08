
#ifndef EXPANDER_H
#define EXPANDER_H

#include "minishell.h"
#include "parse.h"
typedef struct s_ast t_ast;
typedef struct s_shell_context t_shell_context;
t_ast *expand_node(t_ast *node, t_shell_context *sh);


#endif
#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution
# include "../lib/ft_printf/ft_printf.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern unsigned int	g_lastcmd_exit_code;

typedef enum e_redir_type
{
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_APPEND,  //>>
	REDIR_HEREDOC, //<<
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type type; // <, >, >>, << (<< EOF → target = "EOF")
	char *target;      // filename or separate heredoc
	int heredoc_fd;    // for << ，其它类型设 -1
	struct s_redir	*next;
}					t_redir;

typedef struct s_simple_cmd
{
	char **argv; // is a list , we put every cmd in it
	t_redir			*redirects;
}					t_simple_cmd;

typedef struct s_pipeline
{
	t_simple_cmd **cmds; // cmds_count 个 simple_cmd*
	int				cmds_count;
}					t_pipeline;

typedef enum e_ast_type
{
	AST_PIPELINE, // leaf
	AST_AND,      // &&
	AST_OR,       // ||
}					t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_pipeline		*pipeline;
}					t_ast;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*key_value;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

int					builtin_cd(char **argv, t_env *env);
int					builtin_export(char **argv, t_env *env);
int					builtin_unset(char **argv, t_env *env);
int					builtin_exit(char **argv, t_env *env);
int					builtin_echo(char **argv);
int					builtin_pwd(char **argv);
int					builtin_env(t_env *env);
t_pipeline			*build_fake_cmd_table_for_tests(void);
#endif
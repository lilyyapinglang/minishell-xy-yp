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

extern unsigned int			g_lastcmd_exit_code;

/*
typedef enum e_redir_type
{
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_APPEND,  //>>
	REDIR_HEREDOC, //<<
}							t_redir_type;
							*/
// core

typedef struct s_shell
{
	t_env					*env;
	int						last_status;
	bool					in_main_process;
}							t_shell;

// parser //
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR
}							t_ast_type;

typedef struct s_ast_command
{
	char					**args;
}							t_ast_command;

typedef struct s_ast_pipeline
{
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type			operator;
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type			direction;
	struct s_ast			*child;
	char					*file;
}							t_ast_redirection;

typedef struct s_ast_subshell
{
	struct s_ast			*child;
}							t_ast_subshell;

typedef struct s_ast_syntax_error
{
	char					*unexpected_token;
}							t_ast_syntax_error;

typedef struct s_ast
{
	t_ast_type				type;
	union
	{
		t_ast_command		command;
		t_ast_pipeline		pipeline;
		t_ast_logical		logical;
		t_ast_redirection	redirection;
		t_ast_subshell		subshell;
		t_ast_syntax_error	s_error;
	} u_data;
}							t_ast;

// exection
typedef enum e_exitORreturnAfterCurExe
{
	RETURN2PARENT,
	EXITAFTEREXE
}							t_exitORreturnAfterCurExe;

# define READ_END 0
# define WRITE_END 1

typedef struct s_env
{
	char					*key;
	char					*value;
	char					*key_value;
	struct s_env			*prev;
	struct s_env			*next;
}							t_env;

// built-in
int							builtin_cd(char **argv, t_env *env);
int							builtin_export(char **argv, t_env *env);
int							builtin_unset(char **argv, t_env *env);
int							builtin_exit(char **argv, t_env *env);
int							builtin_echo(char **argv);
int							builtin_pwd(char **argv);
int							builtin_env(t_env *env);

// execute

t_ast_command				*build_fake_cmd_table_for_tests(void);
#endif
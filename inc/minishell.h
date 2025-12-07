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
	char *file;        // filename or separate heredoc
	// int             heredoc_fd; // for << ，其它类型设 -1
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char **argv; // is a list , we put every cmd in it
	t_redir			*redirects;
	struct s_cmd *next; // cmd1 -> cmd2 -> cmd3 -> NULL
						// int pipe[2]
}					t_cmd;

typedef struct s_cmd_table
{
	t_cmd			*cmds;
	int				nb_nums;
}					t_cmd_table;

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

#endif
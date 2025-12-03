#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution
# include "../lib/ft_printf/ft_printf.h"
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
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
	int				type;
	char *filename; // delimiter for heredoc
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	int				pipe_in;
	int				pipe_out;
}					t_cmd;

typedef struct s_cmd_table
{
	t_cmd			*cmds;
	int				nb_nums;
}					t_cmd_table;

typedef struct s_exec
{
	int				numOfCmds;
	char			**envp;

}					t_exec;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct g_var
{
	int				singal;
}					t_g_var;

int					builtin_cd(char **argv, t_env *env);
int					builtin_export(char **argv, t_env *env);
int					builtin_unset(char **argv, t_env *env);
int					builtin_exit(char **argv, t_env *env);
int					builtin_echo(char **argv);
int					builtin_pwd(char **argv);
int					builtin_env(t_env *env);

#endif
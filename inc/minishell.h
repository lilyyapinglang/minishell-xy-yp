#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution

typedef enum e_redir_type
{
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_APPEND,  //>>
	REDIR_HEREDOC, //<<
}			t_redir_type;

typedef struct s_redir
{
	int		type;
	char *filename; // delimiter for heredoc
}			t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		pipe_in;
	int		pipe_out;
}			t_cmd;

typedef struct s_cmd_table
{
	t_cmd	*cmds;
	int		nb_nums;
}			t_cmd_table;

typedef struct s_exec
{
	int		numOfCmds;
	char	**envp;

}			t_exec;

#endif
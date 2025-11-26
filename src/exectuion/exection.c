#include "../inc/minishell.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		pipe_in;
	int		pipe_out;
}			t_cmd;

int			apply_input_redir(t_cmd *cmd);

int			apply_output_redir(t_cmd *cmd);

void		apply_all_redirs(t_cmd *cmd);

int			is_buildtin(char *cmd);

int	exec_builtin_in_parent(t_cmd *cmd)
{
	// cd  // export // unset //exit
}

// build executable path from PATH
// check access
// execve
void	callexecve(t_cmd *cmd)
{
	char	*path;

	path = resolve_cmd_path(cmd->argv[0]);
	execve(path, cmd->argv, g_env);
	perror("execve");
	exit(1);
}

void	exec_child(t_cmd *cmd)
{
	// apply_all_redirs(cmd);
	// echo , pwd, env
	callexecve(cmd);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		pid_1;
	int		pid_2;
	int		i;
	t_cmd	single_cmd;

	single_cmd = (t_cmd)malloc(sizeof(t_cmd));
	single_cmd.argv = argv[1];
	t_exec.envp = envp;
	// identify if it's a builtin
	if (is_buildtin(single_cmd.argv[0]))
		return (exec_builtin_in_parent(single_cmd.argv[0]));
	// if not a builtion -> fork ,handle in child process
	pid_1 = getpid();
	pid_2 = fork();
	if (pid_2 == 0)
	{
		exec_child(cmd);
	}
	else
		waitpid(pid, &status, 0);
}
// execute

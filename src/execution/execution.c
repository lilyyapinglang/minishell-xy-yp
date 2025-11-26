#include "../inc/minishell.h"

t_exec		g_t_exec;

int	is_buildtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "pwd", 3)
		|| !ft_strncmp(cmd, "env", 3) || !ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5)
		|| !ft_strncmp(cmd, "exit", 4));
}

int	exec_builtin_in_parent(t_cmd *cmd)
{
	// cd  // export // unset //exit
	if (!ft_strncmp(cmd->argv[0], "cd", 2))
		return (builtin_cd(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "export", 6))
		return (builtin_export(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "unset", 5))
		return (builtin_unset(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "exit", 4))
		return (builtin_exit(cmd->argv));
	return (0);
}
char	*resolve_cmd_path(char *cmd, char **env)
{
	char	**dirs;
	char	*full_path;
	char	*path_ptr;

	path_ptr = NULL;
	while (*env)
	{
		// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
		if (ft_strncmp(*env, "PATH=", 5) == 0)
		{
			// printf("%s\n", *envp);
			path_ptr = strchr(*env, '=') + 1;
			printf("%s\n", path_ptr);
			break ;
		}
		env++;
	}
	if (!path_ptr)
		return (NULL);
	dirs = ft_split(path_ptr, ':');
	if (!dirs)
		return (NULL);
	while (*dirs)
	{
		full_path = ft_strjoin(ft_strjoin(*dirs, "/"), cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		dirs++;
	}
	return (NULL);
}
// build executable path from PATH
// check access
// execve

void	execve_ext(t_cmd *cmd, char **env)
{
	char	*path;

	path = resolve_cmd_path(cmd->argv[0], env);
	if (!path)
	{
		printf("command %s not find in paths\n", cmd->argv[0]);
		exit(1);
	}
	execve(path, cmd->argv, env);
	// perror("execve");
	exit(1);
}

void	exec_child(t_cmd *cmd, char **env)
{
	// apply_all_redirs(cmd); to be done later
	// echo , pwd, env
	// run builtin that is allowed in child (echo, pwd, env)
	if (!ft_strncmp(cmd->argv[0], "echo", 4))
		exit(builtin_echo(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		exit(builtin_pwd(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "env", 3))
		exit(builtin_env(env));
	// otherwise exec external command
	execve_ext(cmd, env);
}

int	exec_single_cmd(t_cmd *cmd, char **env)
{
	pid_t	pid;
	int		status;

	// if builtin command to be run in parent
	if (is_buildtin(cmd->argv[0]))
		return (exec_builtin_in_parent(cmd));
	// need to run in child
	pid = fork();
	if (pid == 0)
	{
		exec_child(cmd, env);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
/*
typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		pipe_in;
	int		pipe_out;
}			t_cmd;
			*/
int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	single_cmd;
	int		exit_status;

	if (argc < 2)
		return (1);
	g_t_exec.numOfCmds = 1;
	g_t_exec.envp = envp;
	single_cmd.argv = &argv[1];
	exit_status = exec_single_cmd(&single_cmd, g_t_exec.envp);
	ft_printf("exit status : %d", exit_status);
}

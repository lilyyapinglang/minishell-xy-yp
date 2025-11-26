#include "../inc/minishell.h"

t_exec	g_t_exec;

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
	char	*path;
	char	**dirs;

	path = getenv("PATH");
	dirs = ft_split(path, ":");
	while (*paths)
	{
	}
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
		printf("command not find in paths\n", cmd->argv[0]);
		exit(127);
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
	callexecve(cmd, env);
}

int	exec_single_cmd(t_cmd *cmd, char **env)
{
	pid_t	pid;
	int		status;

	// if builtin command to be run in parent
	if (is_buildtin(cmd->argv[0]))
		exec_builtin_in_parent(cmd);
	// need to run in child
	pid = folk();
	if (pid = 0)
	{
		exec_child(cmd, env);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	single_cmd;

	g_t_exec.numOfCmds = 1;
	g_t_exec.envp = envp;
	// char	*envp
	// char	*path_ptr;
	// int		pid_1;
	// int		pid_2;
	// int		i;
	// int		status;
	// while (*envp)
	// {
	// 	// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
	// 	if (strncmp(*envp, "PATH", 4) == 0)
	// 	{
	// 		// printf("%s\n", *envp);
	// 		path_ptr = strchr(*envp, '=') + 1;
	// 		printf("%s\n", path_ptr);
	// 	}
	// 	envp++;
	// }
	// single_cmd = (t_cmd)malloc(sizeof(t_cmd));
	// single_cmd.argv = argv[1];
	// t_exec.envp = envp;
	// // identify if it's a builtin
	// if (is_buildtin(single_cmd.argv[0]))
	// 	return (exec_builtin_in_parent(single_cmd.argv[0]));
	// // if not a builtion -> fork ,handle in child process
	// pid_1 = getpid();
	// pid_2 = fork();
	// if (pid_2 == 0) // child
	// {
	// 	exec_child(single_cmd.argv[0]);
	// }
	// else // parent
	// 	waitpid(pid_2, &status, 0);
}



int	is_buildtin(char *cmd)
{
	return (!strcmp(cmd, "echo") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "cd") || !strcmp(cmd, "export") || !strcmp(cmd, "unset")
		|| !strcmp(cmd, "exit"));
}

int	exec_builtin_in_parent(t_cmd *cmd)
{
	// cd  // export // unset //exit
	if (!strcmp(cmd->argv[0], "cd"))
		return (builtin_cd(cmd->argv));
	if (!strcmp(cmd->argv[0], "export"))
		return (builtin_export(cmd->argv));
	if (!strcmp(cmd->argv[0], "unset"))
		return (builtin_unset(cmd->argv));
	if (!strcmp(cmd->argv[0], "exit"))
		return (builtin_exit(cmd->argv));
	return (0);
}
char	*resolve_cmd_path(void)
{
	char	**paths;

	paths = ['bin/'];
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
	// apply_all_redirs(cmd);
	// echo , pwd, env
	// run builtin that is allowed in child (echo, pwd, env)
	if (!strcmp(cmd->argv[0], "echo"))
		exit(builtin_echo(cmd->argv));
	if (!strcmp(cmd->argv[0], "pwd"))
		exit(builtin_pwd(cmd->argv));
	if (!strcmp(cmd->argv[0], "env"))
		exit(builtin_env(env));
	// otherwise exec external command
	callexecve(cmd, env);
}

int	exec_single_cmd(t_cmd *cmd, char **env)
{
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**paths;
	char	*path_ptr;
	t_cmd	single_cmd;

	// int		pid_1;
	// int		pid_2;
	// int		i;
	// int		status;
	while (*envp)
	{
		// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
		if (strncmp(*envp, "PATH", 4) == 0)
		{
			// printf("%s\n", *envp);
			path_ptr = strchr(*envp, '=') + 1;
			printf("%s\n", path_ptr);
		}
		envp++;
	}
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

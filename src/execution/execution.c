#include "../inc/minishell.h"

t_exec		g_t_exec;

int	is_buildtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "pwd", 3)
		|| !ft_strncmp(cmd, "env", 3) || !ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5)
		|| !ft_strncmp(cmd, "exit", 4));
}

char	*resolve_cmd_path(char *cmd, t_env *env)
{
	char	**dirs;
	char	*full_path;
	char	*path_ptr;

	path_ptr = NULL;
	// while (env)
	// {
	// 	// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
	// 	if (ft_strncmp(*env, "PATH=", 5) == 0)
	// 	{
	// 		// printf("%s\n", *envp);
	// 		path_ptr = strchr(*env, '=') + 1;
	// 		// printf("%s\n", path_ptr);
	// 		break ;
	// 	}
	// 	env++;
	// }
	while (env)
	{
		if (ft_strncmp(env->key, "PATH=", 5) == 0)
			path_ptr = env->value;
		env = env->next;
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

char	**env_list_to_envp(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	int		i;

	count = 0;
	tmp = env;
	// Count how many env variables you have
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		int len = strlen(tmp->key) + strlen(tmp->value) + 2; // '=' + '\0'
		envp[i] = malloc(len);
		sprintf(envp[i], "%s=%s", tmp->key, tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	execve_ext(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_str;

	path = resolve_cmd_path(cmd->argv[0], env);
	if (!path)
	{
		printf("command %s not find in paths\n", cmd->argv[0]);
		exit(1);
	}
	// turn t_env into char **
	env_str = env_list_to_envp(env);
	execve(path, cmd->argv, env_str);
	// perror("execve");
	exit(1);
}
// need to guard the length with \0
int	exec_builtin_in_parent(t_cmd *cmd, t_env *env)
{
	// cd  // export // unset //exit
	if (!ft_strncmp(cmd->argv[0], "cd", 2))
		return (builtin_cd(cmd->argv, env));
	if (!ft_strncmp(cmd->argv[0], "export", 6))
		return (builtin_export(cmd->argv, env));
	if (!ft_strncmp(cmd->argv[0], "unset", 5))
		return (builtin_unset(cmd->argv, env));
	if (!ft_strncmp(cmd->argv[0], "exit", 4))
		return (builtin_exit(cmd->argv, env));
	if (!ft_strncmp(cmd->argv[0], "echo", 4))
		exit(builtin_echo(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		exit(builtin_pwd(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "env", 3))
		exit(builtin_env(env));
	return (0);
}

void	exec_child(t_cmd *cmd, t_env *env)
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

// build executable path from PATH
// check access
// execve

int	exec_single_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	// if builtin command to be run in parent
	if (is_buildtin(cmd->argv[0]))
		return (exec_builtin_in_parent(cmd, env));
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

t_env	*dup_env(char **envp)
{
	char	**key_value_split;
	t_env	*new_env_node;
	t_env	*prev;
	t_env	*head;
	int		i;

	prev = NULL;
	head = NULL;
	while (*envp)
	{
		key_value_split = ft_split(*envp, '=');
		if (!key_value_split)
			return (NULL);
		new_env_node = malloc(sizeof(t_env));
		if (!new_env_node)
			return (NULL);
		new_env_node->key = ft_strdup(key_value_split[0]);
		new_env_node->value = ft_strdup(key_value_split[1] ?: "");
		new_env_node->next = NULL;
		new_env_node->prev = prev;
		if (prev)
			prev->next = new_env_node;
		else
			head = new_env_node;
		prev = new_env_node;
		i = 0;
		while (key_value_split[i])
			free(key_value_split[i++]);
		free(key_value_split);
		envp++;
	}
	return (head);
}
int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	single_cmd;
	int		exit_status;
	t_env	*cpy_env;

	if (argc < 2)
		return (1);
	g_t_exec.numOfCmds = 1;
	g_t_exec.envp = envp;
	cpy_env = dup_env(envp); // return pointer to the head node
	printf("i'm here \n");
	single_cmd.argv = &argv[1];
	// exit_status = exec_single_cmd(&single_cmd, g_t_exec.envp);
	// while (cpy_env)
	// {
	// 	printf("key - %s \n", cpy_env->key);
	// 	printf("value - %s\n", cpy_env->value);
	// 	cpy_env = cpy_env->next;
	// }
	exit_status = exec_single_cmd(&single_cmd, cpy_env);
	ft_printf("exit status : %d", exit_status);
}

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
	// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
	while (env)
	{
		if (ft_strncmp(env->key, "PATH", 4) == 0)
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

// Function to resolve relative paths to absolute paths
char	*get_full_path_from_relative(char *relative_path)
{
	char	*cwd;
	char	*full_path;

	// Step 1: Get the current working directory
	cwd = getcwd(NULL, 0);
	// Allocate memory for the current working directory
	if (cwd == NULL)
	{
		perror("getcwd failed");
		return (NULL);
	}
	// Step 2: Calculate the full path by combining cwd with the relative path
	// For example, if cwd is
	//	/home/user and the relative path is ./my_script.sh,
	// the result should be /home/user/my_script.sh
	full_path = malloc(strlen(cwd) + strlen(relative_path) + 2);
	// +2 for '/' and '\0'
	if (full_path == NULL)
	{
		perror("malloc failed");
		free(cwd);
		return (NULL);
	}
	// Step 3: Combine current working directory and relative path
	// If relative path starts with './',
	// append only after removing the './'
	if (relative_path[0] == '.' && relative_path[1] == '/')
	{
		snprintf(full_path, strlen(cwd) + strlen(relative_path), "%s%s", cwd,
			relative_path + 2);
	}
	else
	{
		snprintf(full_path, strlen(cwd) + strlen(relative_path) + 2, "%s/%s",
			cwd, relative_path);
	}
	// Step 4: Clean up and return the full path
	free(cwd); // No longer need the cwd
	return (full_path);
}
void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_str;

	// if it is realtive path
	if (cmd->argv[0][0] == '.')
	{
		ft_printf("here\n");
		path = get_full_path_from_relative(cmd->argv[0]);
	}
	// if it is a absolute path
	else if (cmd->argv[0][0] == '/')
		path = cmd->argv[0];
	// external via PATH
	else
	{
		ft_printf("argv[0]\n", cmd->argv[0]);
		path = resolve_cmd_path(cmd->argv[0], env);
	}
	if (!path)
	{
		printf("command %s not find in paths\n", cmd->argv[0]);
		exit(127);
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
// }
// else
// {
// 	// Handle external command (search in PATH)
// 	char *executable = find_executable(args[0]);
// 	if (executable)
// 	{
// 		execv(executable, args); // Execute external command
// 	}
// 	else
// 	{
// 		fprintf(stderr, "%s: command not found\n", args[0]);
// 	}
// }
// }

// void	execute_external(char **args)
// {
// 	pid_t pid = fork(); // Create a child process
// 	if (pid == 0)
// 	{ // Child process
// 		// Use execvp to run the command in the child process
// 		if (execvp(args[0], args) == -1)
// 		{
// 			perror("execvp failed");
// 		}
// 		exit(EXIT_FAILURE); // If execvp fails, exit the child process
// 	}
// 	else if (pid > 0)
// 	{                          // Parent process
// 		waitpid(pid, NULL, 0); // Wait for the child to finish
// 	}
// 	else
// 	{
// 		perror("fork failed");
// 	}
// }

void	exec_child(t_cmd *cmd, t_env *env)
{
	// apply_all_redirs(cmd); to be done later
	// echo , pwd, env
	// run builtin that is allowed in child (echo, pwd, env)
	// if (!ft_strncmp(cmd->argv[0], "echo", 4))
	// 	exit(builtin_echo(cmd->argv));
	// if (!ft_strncmp(cmd->argv[0], "pwd", 3))
	// 	exit(builtin_pwd(cmd->argv));
	// if (!ft_strncmp(cmd->argv[0], "env", 3))
	// 	exit(builtin_env(env));
	// otherwise exec external command
	execute_external(cmd, env);
}

// void	shell_loop(void)
// {
// 	char	*input;
// 	char	**args;

// 	while (1)
// 	{
// 		printf("minishell> ");
// 		input = read_input();      // Read user input
// 		args = parse_input(input); // Parse input into command + arguments
// 		// Check if it's a built-in command
// 		if (is_builtin(args[0]))
// 		{
// 			execute_builtin(args); // Execute in the parent process
// 		}
// 		else
// 		{
// 			execute_external(args);
// 			// Fork a child process to execute external command
// 		}
// 		free(input);
// 		free(args);
// 	}
// }

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
	if (pid == 0) // child process
		exec_child(cmd, env);
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
	int		i;

	// char	*inpt;
	(void)argc;
	i = 0;
	// while (i < 10)
	// {
	// 	inpt = readline("Minishell> ");
	// 	add_history(inpt);
	// 	printf("%s", inpt);
	// 	printf("\n");
	// 	++i;
	// }
	// return (0);
	g_t_exec.numOfCmds = 1;
	g_t_exec.envp = envp;
	cpy_env = dup_env(envp); // return pointer to the head node
	// printf("i'm here \n");
	single_cmd.argv = &argv[1];
	// exit_status = exec_single_cmd(&single_cmd, g_t_exec.envp);
	// while (cpy_env)
	// {
	// 	printf("key - %s \n", cpy_env->key);
	// 	printf("value - %s\n", cpy_env->value);
	// 	cpy_env = cpy_env->next;
	// }
	exit_status = exec_single_cmd(&single_cmd, cpy_env);
	ft_printf("exec_single_cmd exit status : %d\n", exit_status);
}

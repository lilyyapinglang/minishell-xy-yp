#include "../inc/minishell.h"

void	free_env_list(t_env *head);

int	is_buildtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) || !ft_strncmp(cmd, "pwd", 3)
		|| !ft_strncmp(cmd, "env", 3) || !ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5)
		|| !ft_strncmp(cmd, "exit", 4));
}

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

char	*resolve_cmd_path(char *cmd, t_env *env)
{
	char	**dirs;
	char	*full_path;
	char	*path_ptr;
	int		i;
	char	*mid_path;
	char	**head_ptr;

	i = 0;
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
	if (*path_ptr == '\0')
	{
		ft_putstr_fd("minishell", STDOUT_FILENO);
		ft_putstr_fd(": ", STDOUT_FILENO);
		ft_putstr_fd(cmd, STDOUT_FILENO);
		ft_putstr_fd(": ", STDOUT_FILENO);
		ft_putstr_fd("No such file or directory", STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		exit(127);
	}
	dirs = ft_split(path_ptr, ':');
	if (!dirs)
		return (NULL);
	head_ptr = dirs;
	while (*dirs)
	{
		mid_path = ft_strjoin(*dirs, "/");
		full_path = ft_strjoin(mid_path, cmd);
		free(mid_path);
		// check for execution permission
		if (access(full_path, X_OK) == 0)
		{
			// free dirs up to now
			free_strs(head_ptr);
			return (full_path);
		}
		free(full_path);
		dirs++;
	}
	// free strs too
	free_strs(head_ptr);
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
int	get_file_type(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == 0)
	{
		// 文件存在，buf 包含文件信息
		// it is a file
		if (S_ISREG(buf.st_mode))
			return (1);
		// it is a directory;
		else if (S_ISDIR(buf.st_mode))
			return (2);
		else
			return (3);
	}
	return (-1);
}
void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_str;

	if (!(cmd->argv[0]) || ft_strchr(cmd->argv[0], '/'))
	{
		// permission denied
		// the direcotry
		path = cmd->argv[0];
		if (get_file_type(path) == -1)
		{
			ft_putstr_fd("minishell: no such file or directory: ",
				STDOUT_FILENO);
			ft_putstr_fd(path, STDOUT_FILENO);
			exit(127);
		}
		else if (get_file_type(path) == 2)
		{
			ft_putstr_fd("minishell: ", STDOUT_FILENO);
			ft_putstr_fd(path, STDOUT_FILENO);
			ft_putstr_fd(": Is a directory\n", STDOUT_FILENO);
			exit(126);
		}
		else if (access(path, X_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDOUT_FILENO);
			ft_putstr_fd(path, STDOUT_FILENO);
			ft_putstr_fd(": Permission denied\n", STDOUT_FILENO);
			exit(126);
		}
	}
	// external via PATH
	else
		path = resolve_cmd_path(cmd->argv[0], env);
	if (!path)
	{
		ft_printf("%s: command not found\n", cmd->argv[0]);
		free_env_list(env);
		exit(127);
	}
	// turn t_env into char **
	env_str = env_list_to_envp(env);
	execve(path, cmd->argv, env_str);
	// perror("execve");
	free(path);
	free_strs(env_str);
	free_env_list(env);
	exit(0);
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
		return (builtin_echo(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		return (builtin_pwd(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "env", 3))
		return (builtin_env(env));
	return (0);
}

void	exec_child(t_cmd *cmd, t_env *env)
{
	execute_external(cmd, env);
}

int	exec_single_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	// if builtin command to be run in parent
	if (!cmd)
		return (1);
	if (is_buildtin(cmd->argv[0]))
		return (exec_builtin_in_parent(cmd, env));
	// need to run in child
	pid = fork();
	if (pid == 0) // child process
		exec_child(cmd, env);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

void	free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = tmp;
	}
}

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
		{
			free_env_list(head);
			return (NULL);
		}
		new_env_node = malloc(sizeof(t_env));
		if (!new_env_node)
		{
			free_strs(key_value_split);
			free_env_list(head);
			return (NULL);
		}
		new_env_node->key_value = *envp;
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
		free_strs(key_value_split);
		envp++;
	}
	return (head);
}

// cat a | grep hi | wc -l
/*
nb_cmds = 3

cmd[0]:
  argv = ["cat", "a", NULL]
  redirs = []

cmd[1]:
  argv = ["grep", "hi", NULL]
  redirs = []

cmd[2]:
  argv = ["wc", "-l", NULL]
  redirs = []

*/

void	create_test_cmd_table(void)
{
	t_cmd_table	*cmd_table;
	t_cmd		*cmd_1;
	t_cmd		*head;
	t_cmd		*cmd_2;
	t_cmd		*cmd_3;

	cmd_1 = malloc(sizeof(t_cmd));
	head = cmd_1;
	cmds->argv = {"cat", "a", NULL};
	t_cmd_table->nb_nums = 3;
}
int	main(int argc, char *argv[], char *envp[])
{
	t_cmd		single_cmd;
	int			exit_status;
	t_env		*cpy_env_list;
	t_cmd_table	*cmd_table;
	int			i;
	int			fildes[2];
	int			READ_END;
	int			WRITE_END;
	pid_t		pid;

	READ_END = 0;
	WRITE_END = 1;
	cmd_table = NULL;
	(void)argc;
	i = 0;
	cpy_env_list = dup_env(envp); // return pointer to the head node
	if (!cpy_env_list)
		return (1);
	if (cmd_table->nb_nums) // nb_nums =0;
		return (1);
	// eg: cat a.txt | grep hi | wc -l
	if (cmd_table->cmds)
	{
		// it exist the head of the commands nb_nums=3
		while (i < cmd_table->nb_nums)
		{
			if (i != cmd_table->nb_nums - 1) // not the last cmd,
												// i will create pipe
			{
				// Data can be written to the file descriptor fildes[1] and read from  the
				// file  descriptor  fildes[0]
				// fildes[1] write, fildes[0] read, first in first out
				pipe(fildes); // empty pipe, got 2 fd
				pid = fork();
				// create a chile process to execute current command
				if (pid == 0) // child process
				{
					if (cmd_table->cmds[i - 1])
						// it has previous cmd,
						// read from last fd_read instead of standard input
						dup2(fildes[READ_END], STDIN_FILENO);
					// read from the previoud read end
					dup2(fildes[WRITE_END], STDOUT_FILENO);
					// close(fildes[READ_END]);
					// no thing to read from  so close?
					// close(fildes[WRITE_END]);
					exec_child(cmd_table->cmds[i], fildes, cpy_env_list);
					ft_printf("Failed to execute %s \n", cmd_table->cmds[i]);
					exit(1);
				}
				else // parent process
				{
					close(fd[READ_END]);
					close(fd[WRITE_END]);
					waitpid(pid, &status, 0);
				}
			}
			// last command what to do ?
			// i just need to read from the last fd read_end and standard output
			else if (i == cmd_table->nb_nums - 1) // last command
			{
				dup2(fildes[READ_END], STDIN_FILENO);
				close(fd[WRITE_END]);
				close(fd[READ_END]);
				exec_child(cmd_table->cmds[i], fildes, cpy_env_list);
				ft_printf("Failed to execute last cmd %s \n",
					cmd_table->cmds[i]);
				exit(1);
			}
		}
	}
	single_cmd.argv = &argv[1];
	exit_status = exec_single_cmd(&single_cmd, cpy_env_list);
	ft_printf("exec_single_cmd exit status : %d\n", exit_status);
	free_env_list(cpy_env_list);
	return (exit_status);
}

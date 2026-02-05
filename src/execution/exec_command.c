#include "../inc/minishell.h"

/*

builtin 是否在父进程/子进程跑，取决于调用 execute_command() 的那个进程是谁？”
**如果当前进程是“主 shell”，
那么：

external command → 必须 fork

stateful builtin → 不能 fork

stateless builtin → 可 fork 可不 fork**
*/

void	free_strs(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

char	**env_list_to_envp(t_list *env_list)
{
	int			count;
	int			i;
	t_list		*node;
	t_env_var	*ev;
	char		**envp;
	const char	*val;
	size_t		name_len;
	size_t		val_len;

	count = 0;
	i = 0;
	/* 1) count exported */
	node = env_list;
	while (node)
	{
		ev = env_var_from_node(node);
		if (ev && ev->exported && ev->name)
			count++;
		node = node->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	/* 2) build envp */
	node = env_list;
	while (node)
	{
		ev = env_var_from_node(node);
		if (ev && ev->exported && ev->name)
		{
			name_len = ft_strlen(ev->name);
			val = ev->value ? ev->value : "";
			val_len = ft_strlen(val);
			envp[i] = malloc(name_len + 1 + val_len + 1);
			if (!envp[i])
			{
				/* rollback */
				while (i > 0)
					free(envp[--i]);
				free(envp);
				return (NULL);
			}
			ft_memcpy(envp[i], ev->name, name_len);
			envp[i][name_len] = '=';
			ft_memcpy(envp[i] + name_len + 1, val, val_len);
			envp[i][name_len + 1 + val_len] = '\0';
			i++;
		}
		node = node->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*resolve_cmd_path(char *cmd, t_shell_context *sh_ctx)
{
	char		**dirs;
	char		*full_path;
	char		*path_ptr;
	char		*mid_path;
	char		**head_ptr;
	t_list		*env;
	t_env_var	*env_var;

	path_ptr = NULL;
	env = sh_ctx->env;
	// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
	while (env)
	{
		env_var = env_var_from_node(env);
		if (!env_var)
		{
			env = env->next;
			continue ;
		}
		if (ft_strcmp(env_var->name, "PATH") == 0)
			path_ptr = env_var->value;
		env = env->next;
	}
	if (!path_ptr)
	{ // try ./cmd
		full_path = ft_strjoin("./", cmd);
		return (full_path);
	}
	if (*path_ptr == '\0')
		// return (print_msg_n_return(127, cmd, NULL,
		//	"No such file or directory"));
		return (NULL);
	dirs = ft_split(path_ptr, ':');
	if (!dirs)
		return (NULL);
	head_ptr = dirs;
	while (*dirs)
	{
		mid_path = ft_strjoin(*dirs, "/");
		if (!mid_path)
		{
			free_strs(head_ptr);
			return (NULL);
		}
		full_path = ft_strjoin(mid_path, cmd);
		free(mid_path);
		if (!full_path)
		{
			free_strs(head_ptr);
			return (NULL);
		}
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

int	execve_errno_to_status(int err)
{
	if (err == ENOENT)
		return (127);
	return (126);
}
int	execute_external(t_ast_command *cmd, t_shell_context *sh_ctx)
{
	int			status;
	char		*path;
	char		**envp;
	struct stat	st;

	// cmd->args[0] ==""
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	// contains '/'
	if (ft_strchr(cmd->args[0], '/'))
	{
		// permission denied
		// the direcotry
		path = cmd->args[0];
		if (stat(cmd->args[0], &st) == -1)
		{
			path = cmd->args[0];
			if (errno == ENOENT)
				return (print_errno_n_return(127, cmd->args[0], NULL, errno));
			if (errno == EACCES)
				return (print_errno_n_return(126, cmd->args[0], NULL, errno));
			return (print_errno_n_return(126, cmd->args[0], NULL, errno));
		}
		if (S_ISDIR(st.st_mode))
			return (print_msg_n_return(126, path, NULL, "Is a directory"));
		if (access(path, X_OK) == -1)
			return (print_msg_n_return(126, path, NULL, "Permission denied"));
	}
	// external via PATH
	else
	{
		if (*cmd->args[0] == '\0')
			return (print_msg_n_return(127, cmd->args[0], NULL,
					"command not found"));
		// if (ft_strcmp(cmd->args[0], "."))
		// 	return (print_msg_n_return(2, cmd->args[0], NULL,
		// 			"filename argument required"));
		path = resolve_cmd_path(cmd->args[0], sh_ctx);
		if (!path)
			return (print_msg_n_return(127, cmd->args[0], NULL,
					"command not found"));
	}
	envp = env_list_to_envp(sh_ctx->env);
	if (!envp)
		return (1);
	execve(path, cmd->args, envp);
	status = execve_errno_to_status(errno);
	free_strs(envp);
	if (path != cmd->args[0])
		free(path);
	return (print_errno_n_return(status, cmd->args[0], NULL, errno));
}

int	wait_status_to_shell_status(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	if (WIFSIGNALED(wait_status))
		return (128 + WTERMSIG(wait_status));
#ifdef WIFSTOPPED
	if (WIFSTOPPED(wait_status))
		return (128 + WSTOPSIG(wait_status));
#endif
	// Fallback: should not happen in normal minishell execution
	return (1);
}

void	report_child_termination_signal(int wait_status, const char *cmd_name,
		t_shell_context *ctx)
{
	int	sig;

	(void)cmd_name;
	// only parent / interactive shell should print
	if (ctx && ctx->in_main_process == false)
		return ;
	if (!WIFSIGNALED(wait_status))
		return ;
	sig = WTERMSIG(wait_status);
	if (sig == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (sig == SIGQUIT)
	{
#ifdef WCOREDUMP
		if (WCOREDUMP(wait_status))
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else
			ft_putstr_fd("Quit\n", STDERR_FILENO);
#else
		ft_putstr_fd("Quit\n", STDERR_FILENO);
#endif
	}
}

int	fork_and_run_cmd_in_child(t_ast *node, t_shell_context *sh_ctx)
{
	pid_t	pid;
	int		wait_status;

	// int		status;
	pid = fork();
	if (pid < 0)
		return (print_errno_n_return(1, "fork", NULL, errno));
	if (pid == 0)
	{
		sh_ctx->in_main_process = false;
		set_signal_in_exe_child_process();
		/// reuse run in child logic
		// status = execute(node, RUN_IN_CHILD, sh_ctx);
		// printf("i'm in fork_and_run_in_child, before execute \n");
		// printf("node now is: %s\n", node->u_data.command.args[1]);
		execute(node, RUN_IN_CHILD, sh_ctx);
		// printf("i'm in fork_and_run_in_child, after execute \n");
		exit(sh_ctx->last_status); // should be unreachable, safety net
	}
	while (waitpid(pid, &wait_status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		return (print_errno_n_return(1, "waitpid", NULL, errno));
	}
	report_child_termination_signal(wait_status, NULL, sh_ctx);
	return (wait_status_to_shell_status(wait_status));
}

int	execute_command(t_ast *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int				status;
	bool			isbuiltin;
	t_ast_command	*cmd;

	cmd = &node->u_data.command;
	status = EXIT_SUCCESS;
	isbuiltin = false;
	// check if it is built-in
	// if it is built-in like cd, export, unset, ecit et,
	// run directly in currentg process
	// if not buitle-in , like ls , grep,
	// handover to fork_command decide how to run it
	// builtin 总是在“当前进程”执行（谁调用它，它就在哪个进程里跑）
	if (!cmd->args || !cmd->args[0])
		return (EXIT_SUCCESS);
	isbuiltin = is_builtin(cmd->args[0]);
	// already in child process, must not fork
	if (exe_ctx == RUN_IN_CHILD)
	{
		if (isbuiltin)
		{
			// match the built_in name with built_in function name
			status = execute_builtin(cmd, sh_ctx);
			return (status);
		}
		// esle it is not built in, execve rewrite current process
		// external: exec; if fails, return proper status (126/127/1)
		return (execute_external(cmd, sh_ctx));
		// execve(get_cmd_path(cmd->args[0], sh_ctx), cmd->args,
		// 	sh_ctx->env, sh_ctx);
		// return (127);
	}
	else if (exe_ctx == RUN_FORK_WAIT)
	{
		// printf("I am in run_fork_wait\n ");
		return (fork_and_run_cmd_in_child(node, sh_ctx));
	}
	// has to be executed in parent process,shell process
	else if (exe_ctx == RUN_IN_SHELL)
	{
		// stateful builti has to be run in parent ,
		//	otherwise change will not take effect
		if (isbuiltin && is_stateful_builtin(cmd->args[0]))
			return (execute_builtin(cmd, sh_ctx));
	}
	// need to fork , parent fork, child exe as run in child, parent wait
	// default folk
	// printf("I am in default fork \n ");
	return (fork_and_run_cmd_in_child(node, sh_ctx));
}

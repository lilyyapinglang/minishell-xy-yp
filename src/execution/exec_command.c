/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:06:28 by ylang             #+#    #+#             */
/*   Updated: 2026/02/27 16:55:31 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	count_exported_env_vars(t_list *node)
{
	t_env_var	*env_var;
	int			count;

	count = 0;
	while (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->exported && env_var->name)
			count++;
		node = node->next;
	}
	return (count);
}
/* 1) count exported */
/* 2) build envp */
/* rollback */
char	**env_list_to_envp(t_list *env_list)
{
	int			i;
	t_list		*node;
	t_env_var	*env_var;
	char		**envp;
	const char	*val;
	size_t		name_len;
	size_t		val_len;

	i = 0;
	node = env_list;
	envp = malloc(sizeof(char *) * (count_exported_env_vars(node) + 1));
	if (!envp)
		return (NULL);
	node = env_list;
	while (node)
	{
		env_var = env_var_from_node(node);
		if (env_var&& env_var->exported && env_var->name)
		{
			name_len = ft_strlen(env_var->name);
			val = env_var->value ? env_var->value : "";
			val_len = ft_strlen(val);
			envp[i] = malloc(name_len + 1 + val_len + 1);
			if (!envp[i])
			{
				while (i > 0)
					free(envp[--i]);
				free(envp);
				return (NULL);
			}
			ft_memcpy(envp[i], env_var->name, name_len);
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
// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
// try ./cmd
// check for execution permission
// free dirs up to now
// free strs too
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
	{
		full_path = ft_strjoin("./", cmd);
		return (full_path);
	}
	if (*path_ptr == '\0')
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
		if (access(full_path, X_OK) == 0)
		{
			free_strs(head_ptr);
			return (full_path);
		}
		free(full_path);
		dirs++;
	}
	free_strs(head_ptr);
	return (NULL);
}

int	execve_errno_to_status(int err)
{
	if (err == ENOENT)
		return (127);
	return (126);
}
int	validate_command(t_ast_command *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], ".") == 0)
		return (print_msg_n_return(127, cmd->args[0], NULL,
				"filename argument required"));
	if (ft_strcmp(cmd->args[0], "..") == 0)
		return (print_msg_n_return(127, cmd->args[0], NULL,
				"command not found"));
	if (*cmd->args[0] == '\0')
		return (print_msg_n_return(127, cmd->args[0], NULL,
				"command not found"));
	return (-1);
}

int	check_valid_path(char *path, char *cmd_name)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (errno == ENOENT)
			return (print_errno_n_return(127, path, NULL, errno));
		if (errno == EACCES)
			return (print_errno_n_return(126, path, NULL, errno));
		if (errno == EISDIR)
			return (print_errno_n_return(126, path, NULL, errno));
		if (errno == EFAULT)
			return (print_msg_n_return(127, cmd_name, NULL,
					"command not found"));
		return (print_errno_n_return(127, path, NULL, errno));
	}
	return (-1);
}

// cmd->arg[0]=="."
// cmd->args[0] ==".."
// cmd->args[0] ==""
// contains '/', search for cmd from current directory
// permission denied
// the direcotry
// no / present, so need to search and execute external via PATH,
int	execute_external(t_ast_command *cmd, t_shell_context *sh_ctx)
{
	int		status;
	char	*path;
	char	**envp;

	status = validate_command(cmd);
	if (status != -1)
		return (status);
	if (ft_strchr(cmd->args[0], '/'))
		path = cmd->args[0];
	else
		path = resolve_cmd_path(cmd->args[0], sh_ctx);
	status = check_valid_path(path, cmd->args[0]);
	if (status != -1)
		return (status);
	envp = env_list_to_envp(sh_ctx->env);
	if (!envp)
		return (print_msg_n_return(1, cmd->args[0], NULL,
				"fail to convert env_list to envp"));
	execve(path, cmd->args, envp);
	status = execve_errno_to_status(errno);
	free_strs(envp);
	if (path != cmd->args[0])
		free(path);
	return (print_errno_n_return(status, cmd->args[0], NULL, errno));
}

// Fallback: should not happen in normal minishell execution
int	wait_status_to_shell_status(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	if (WIFSIGNALED(wait_status))
		return (128 + WTERMSIG(wait_status));
	if (WIFSTOPPED(wait_status))
		return (128 + WSTOPSIG(wait_status));
	return (1);
}
// only parent / interactive shell should print

void	report_child_termination_signal(int wait_status, const char *cmd_name,
		t_shell_context *ctx)
{
	int	sig;

	(void)cmd_name;
	if (ctx && ctx->in_main_process == false)
		return ;
	if (!WIFSIGNALED(wait_status))
		return ;
	sig = WTERMSIG(wait_status);
	if (sig == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (sig == SIGQUIT)
	{
		if (WCOREDUMP(wait_status))
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else
			ft_putstr_fd("Quit\n", STDERR_FILENO);
		ft_putstr_fd("Quit\n", STDERR_FILENO);
	}
}

// int		status;
/// reuse run in child logic
// status = execute(node, RUN_IN_CHILD, sh_ctx);
// printf("i'm in fork_and_run_in_child, before execute \n");
// printf("node now is: %s\n", node->u_data.command.args[1]);
// printf("i'm in fork_and_run_in_child, after execute \n");
// should be unreachable, safety net
int	fork_and_run_cmd_in_child(t_ast *node, t_shell_context *sh_ctx)
{
	pid_t	pid;
	int		wait_status;

	pid = fork();
	if (pid < 0)
		return (print_errno_n_return(1, "fork", NULL, errno));
	if (pid == 0)
	{
		sh_ctx->in_main_process = false;
		set_signal_in_exe_child_process();
		execute(node, RUN_IN_CHILD, sh_ctx);
		exit(sh_ctx->last_status);
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

// 这里理论上不应该发生（外面已经判断 is_builtin）
// 但为了鲁棒性，返回 1 或 0 都行；通常返回 1 更像“内部错误”
// 注意：builtin 函数自己负责打印错误并返回对应 statusms
// execute_builtin 不要 exit（除了 builtin_exit 自己 exit）
int	execute_builtin(t_ast_command *cmd, t_shell_context *ctx)
{
	t_builtin_func	func;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	func = lookup_builtin_func(cmd->args[0]);
	if (!func)
		return (1);
	return (func(cmd->args, ctx));
}
// check if it is built-in
// if it is built-in like cd, export, unset, ecit et,
// run directly in currentg process
// if not buitle-in , like ls , grep,
// handover to fork_command decide how to run it
// builtin 总是在“当前进程”执行（谁调用它，它就在哪个进程里跑）
// already in child process, must not fork
// match the built_in name with built_in function name
// esle it is not built in, execve rewrite current process
// external: exec; if fails, return proper status (126/127/1)
// execve(get_cmd_path(cmd->args[0], sh_ctx), cmd->args,
// 	sh_ctx->env, sh_ctx);
// return (127);
// printf("I am in run_fork_wait\n ");
// has to be executed in parent process,shell process
// stateful builti has to be run in parent ,
//	otherwise change will not take effect
// need to fork , parent fork, child exe as run in child, parent wait
// default folk
// printf("I am in default fork \n ");
int	execute_command(t_ast *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	bool			isbuiltin;
	t_ast_command	*cmd;

	cmd = &node->u_data.command;
	isbuiltin = false;
	if (!cmd->args || !cmd->args[0])
		return (EXIT_SUCCESS);
	isbuiltin = is_builtin(cmd->args[0]);
	if (exe_ctx == RUN_IN_CHILD)
	{
		if (isbuiltin)
			return (execute_builtin(cmd, sh_ctx));
		return (execute_external(cmd, sh_ctx));
	}
	else if (exe_ctx == RUN_FORK_WAIT)
		return (fork_and_run_cmd_in_child(node, sh_ctx));
	else if (exe_ctx == RUN_IN_SHELL)
	{
		if (isbuiltin && is_stateful_builtin(cmd->args[0]))
			return (execute_builtin(cmd, sh_ctx));
	}
	return (fork_and_run_cmd_in_child(node, sh_ctx));
}

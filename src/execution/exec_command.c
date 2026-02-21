/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:40:16 by ylang             #+#    #+#             */
/*   Updated: 2026/02/21 23:47:27 by ylang            ###   ########.fr       */
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

int	count_exported_env_vars(t_list *env_list)
{
	int			count;
	t_env_var	*env_var;
	t_list		*node;

	node = env_list;
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

void	add_one_env(t_env_var *env_var, int i, t_shell_context *sh_ctx,
		char **envp)
{
	int		name_len;
	int		val_len;
	char	*val;

	name_len = ft_strlen(env_var->name);
	if (env_var->value)
		val = env_var->value;
	else
		val = "";
	val_len = ft_strlen(val);
	envp[i] = calloc_s(name_len + 1 + val_len + 1, sizeof(char), ALLOC_PROMPT,
			sh_ctx);
	ft_memcpy(envp[i], env_var->name, name_len);
	envp[i][name_len] = '=';
	ft_memcpy(envp[i] + name_len + 1, val, val_len);
	envp[i][name_len + 1 + val_len] = '\0';
}
/* 1) count exported */
/* 2) build envp */
/* rollback */

char	**build_envp_or_fail(t_list *env_list, t_shell_context *sh_ctx)
{
	int			i;
	t_list		*node;
	t_env_var	*env_var;
	char		**envp;

	i = 0;
	node = env_list;
	envp = calloc_s(count_exported_env_vars(env_list) + 1, sizeof(char *),
			ALLOC_PROMPT, sh_ctx);
	while (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->exported && env_var->name)
			add_one_env(env_var, i++, sh_ctx, envp);
		node = node->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*search_path_env(t_list *env)
{
	t_env_var	*env_var;
	char		*path_ptr;

	path_ptr = NULL;
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
	return (path_ptr);
}

char	*combine_path(char **dirs, t_shell_context *sh_ctx, char *cmd)
{
	char	*mid_path;
	char	*full_path;

	if (!dirs || !cmd)
		return (NULL);
	while (*dirs)
	{
		mid_path = strjoin_s(*dirs, "/", ALLOC_PROMPT, sh_ctx);
		if (!mid_path)
			return (NULL);
		full_path = strjoin_s(mid_path, cmd, ALLOC_PROMPT, sh_ctx);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		dirs++;
	}
	return (NULL);
}

// PATH=/home/ylang/bin:/home/ylang/bin:/usr/local/sbin:
// usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/
// games:/usr/local/games:/snap/bin
// try ./cmd
// return (print_msg_n_return(127, cmd, NULL,
//	"No such file or directory"));
// check for execution permission
// free dirs up to now
// free strs too

char	*resolve_cmd_path(char *cmd, t_shell_context *sh_ctx)
{
	char	**dirs;
	char	*path_ptr;
	char	**head_ptr;
	t_list	*env;

	env = sh_ctx->env;
	path_ptr = search_path_env(env);
	if (!path_ptr || *path_ptr == '\0')
		return (NULL);
	dirs = split_s(path_ptr, ':', ALLOC_PROMPT, sh_ctx);
	head_ptr = dirs;
	return (combine_path(head_ptr, sh_ctx, cmd));
}

// 这里理论上不应该发生（外面已经判断 is_builtin）
// 但为了鲁棒性，返回 1 或 0 都行；通常返回 1 更像“内部错误”
// 注意：builtin 函数自己负责打印错误并返回对应 status
// execute_builtin 不要 exit（除了 builtin_exit 自己 exit）

int	execute_builtin(t_ast_command *cmd, t_shell_context *sh_ctx)
{
	t_builtin_func	func;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	func = lookup_builtin_func(cmd->args[0]);
	if (!func)
		return (1);
	return (func(cmd->args, sh_ctx));
}

int	validate_command(t_ast_command *cmd)
{
	char	*name;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	name = cmd->args[0];
	if (ft_strcmp(name, ".") == 0)
		return (print_msg_n_return(127, name, NULL,
				"filename argument required"));
	if (ft_strcmp(name, "..") == 0 || name[0] == '\0')
		return (print_msg_n_return(127, name, NULL, "command not found"));
	return (-1);
}

char	*resolve_exection_path(char *name, t_shell_context *sh_ctx)
{
	char	*path;

	if (ft_strchr(name, '/'))
		return (name);
	path = resolve_cmd_path(name, sh_ctx);
	return (path);
}

static int	map_exec_errno(int err)
{
	if (err == ENOENT)
		return (127);
	if (err == EACCES)
		return (126);
	if (err == ENOTDIR)
		return (127);
	return (126);
}

int	exec_and_handle(char *path, t_ast_command *cmd, char **envp)
{
	int	status;

	execve(path, cmd->args, envp);
	status = map_exec_errno(errno);
	return (print_errno_n_return(status, cmd->args[0], NULL, errno));
}

// cmd->arg[0]=="."
// cmd->args[0] ==".."
// cmd->args[0] ==""
// contains '/', search for cmd from current directory
// permission denied
// the direcotry
// no / present, so need to search and execute external via PATH,
// case 1: command contains /
// case 2: command does NOT contain /
int	execute_external(t_ast_command *cmd, t_shell_context *sh_ctx)
{
	int		status;
	char	*path;
	char	**envp;

	status = validate_command(cmd);
	if (status != -1)
		return (status);
	path = resolve_exection_path(cmd->args[0], sh_ctx);
	if (!path)
		return (print_msg_n_return(127, cmd->args[0], NULL,
				"command not found"));
	envp = build_envp_or_fail(sh_ctx->env, sh_ctx);
	if (!envp)
		return (1);
	status = exec_and_handle(path, cmd, envp);
	return (status);
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
		t_shell_context *sh_ctx)
{
	int	sig;

	(void)cmd_name;
	if (sh_ctx && sh_ctx->in_main_process == false)
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
	}
}

// int		status;
/// reuse run in child logic
// status = execute(node, RUN_IN_CHILD, sh_ctx);
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
// // Default: fork child for external or stateless builtin
int	execute_command(t_ast *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	bool			isbuiltin;
	t_ast_command	*cmd;

	if (!node)
		return (EXIT_FAILURE);
	cmd = &node->u_data.command;
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

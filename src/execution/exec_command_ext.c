/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_ext.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:38:48 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/27 17:52:07 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

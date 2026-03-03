/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:55:15 by lilypad           #+#    #+#             */
/*   Updated: 2026/03/03 17:58:58 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	close_retry(int fd)
{
	while (close(fd) == -1)
		if (errno != EINTR)
			return (-1);
	return (0);
}

static int	restore_fd(int saved_fd, int std_fd)
{
	if (dup2(saved_fd, std_fd) == -1)
	{
		close_retry(saved_fd);
		return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
	}
	if (close_retry(saved_fd) == -1)
		return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
	return (EXIT_SUCCESS);
}

 bool	should_fork_on_redirection(t_ast_redirection *redir, t_exec_context ctx)
{
	t_ast_command	*cmd;

	if (ctx != RUN_IN_SHELL || !redir || !redir->exe_child
		|| redir->exe_child->type != AST_COMMAND)
		return (false);
	cmd = &redir->exe_child->u_data.command;
	return (cmd->args && cmd->args[0] && (!is_builtin(cmd->args[0])
			|| !is_stateful_builtin(cmd->args[0])));
}

int redirect_and_execute(t_ast_redirection *node, t_redir_exec redir,
	t_exec_context exe_ctx, t_shell_context *sh_ctx)
{
	int status;
	int original_fd;

	if (exe_ctx == RUN_IN_CHILD)
	{
		if (dup2(redir.new_fd, redir.std_fd) == -1
			|| close_retry(redir.new_fd) == -1)
			return (print_errno_n_return(EXIT_FAILURE, "dup2/close", NULL,
					errno));
		return (execute(node->exe_child, RUN_IN_CHILD, sh_ctx));
	}
	original_fd = dup(redir.std_fd);
	if (original_fd == -1 || dup2(redir.new_fd, redir.std_fd) == -1
		|| close_retry(redir.new_fd) == -1)
	{
		close_retry(redir.new_fd);
		close_retry(original_fd);
		return (print_errno_n_return(EXIT_FAILURE, "dup2/close", NULL, errno));
	}
	status = execute(node->exe_child, exe_ctx, sh_ctx);
	if (restore_fd(original_fd, redir.std_fd) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 17:30:59 by ylang             #+#    #+#             */
/*   Updated: 2026/03/03 17:58:00 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exe_redirect_input(t_ast_redirection *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int				fd;
	t_redir_exec	redir;

	fd = open(node->file_path, O_RDONLY);
	if (fd == -1)
		return (print_errno_n_return(1, "minishell", node->file_path, errno));
	redir.new_fd = fd;
	redir.std_fd = STDIN_FILENO;
	return (redirect_and_execute(node, redir, exe_ctx, sh_ctx));
}

int	exe_redirect_output(t_ast_redirection *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int				fd;
	t_redir_exec	redir;
	int				flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(node->file_path, flags, 0644);
	if (fd == -1)
		return (print_errno_n_return(1, "minishell", node->file_path, errno));
	redir.new_fd = fd;
	redir.std_fd = STDOUT_FILENO;
	return (redirect_and_execute(node, redir, exe_ctx, sh_ctx));
}

int	execute_redirect_append_output(t_ast_redirection *node,
		t_exec_context exe_ctx, t_shell_context *sh_ctx)
{
	int				fd;
	t_redir_exec	redir;
	int				flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(node->file_path, flags, 0644);
	if (fd == -1)
		return (print_errno_n_return(1, "minishell", node->file_path, errno));
	redir.new_fd = fd;
	redir.std_fd = STDOUT_FILENO;
	return (redirect_and_execute(node, redir, exe_ctx, sh_ctx));
}

int	execute_redirection(t_ast *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	t_ast_redirection	*redir_node;

	redir_node = &node->u_data.redirection;
	if (should_fork_on_redirection(redir_node, exe_ctx))
		return (fork_and_run_cmd_in_child(node, sh_ctx));
	if (redir_node->redir_type == REDIR_INPUT
		|| redir_node->redir_type == REDIR_HEREDOC)
		return (exe_redirect_input(redir_node, exe_ctx, sh_ctx));
	if (redir_node->redir_type == REDIR_OUTPUT)
		return (exe_redirect_output(redir_node, exe_ctx, sh_ctx));
	if (redir_node->redir_type == REDIR_APPEND)
		return (execute_redirect_append_output(redir_node, exe_ctx, sh_ctx));
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 17:30:59 by ylang             #+#    #+#             */
/*   Updated: 2026/02/20 17:46:45 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	close_retry(int fd)
{
	while (close(fd) == -1)
	{
		if (errno == EINTR)
			continue ;
		return (-1);
	}
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

static bool	should_fork_on_redirection(t_ast_redirection *redir_node,
		t_exec_context exe_ctx)
{
	t_ast_command	*cmd;

	if (exe_ctx != RUN_IN_SHELL || !redir_node || !redir_node->exe_child)
		return (false);
	if (redir_node->exe_child->type != AST_COMMAND)
		return (false);
	cmd = &redir_node->exe_child->u_data.command;
	if (!cmd->args || !cmd->args[0])
		return (false);
	if (!is_builtin(cmd->args[0]))
		return (true);
	return (!is_stateful_builtin(cmd->args[0]));
}

/*
cat < input.txt
redirection: let stdin point to input.txt
child : cat
//child ──> AST_COMMAND / AST_PIPELINE / AST_SUBSHELL / ...
redirect_input(redir):
1.把 stdin 改成 file
2.执行 redir->child
3. 把 stdin 改回原样
*/
// fd point to the input file or temp file, fd =3 , 4 5..
// now i have a file pipe, not yet stdin (fd=0)
// let orignal_stdin point to the current stdin, which is terminal input,
// because we are doing to chagne stdin,
// so have to save it becasue we need to return to terminal prompt.
// link stdin behavior to inputfile
// after dup2, fd=0 already point to file, input_fd becomes a useless alias
// after executing child, need to change stdin back to terminal
// use backup orignal_stdio to link stdin(0) to original places
// after recover to terminal, close the temp fd

int	exe_redirect_input(t_ast_redirection *redir_node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int	input_fd;
	int	status;
	int	original_stdin;

	input_fd = open(redir_node->file_path, O_RDONLY);
	if (input_fd == -1)
		return (print_errno_n_return(1, "minishell", redir_node->file_path,
				errno));
	if (exe_ctx == RUN_IN_CHILD)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			close_retry(input_fd);
			return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
		}
		if (close_retry(input_fd) == -1)
			return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
		return (execute(redir_node->exe_child, RUN_IN_CHILD, sh_ctx));
	}
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		close_retry(input_fd);
		return (print_errno_n_return(EXIT_FAILURE, "dup", NULL, errno));
	}
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		close_retry(input_fd);
		close_retry(original_stdin);
		return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
	}
	if (close_retry(input_fd) == -1)
	{
		restore_fd(original_stdin, STDIN_FILENO);
		return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
	}
	status = execute(redir_node->exe_child, exe_ctx, sh_ctx);
	if (restore_fd(original_stdin, STDIN_FILENO) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (status);
}

int	exe_redirect_output(t_ast_redirection *redir_node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int	output_fd;
	int	status;
	int	original_stdout;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	output_fd = open(redir_node->file_path, flags, 0644);
	if (output_fd == -1)
		return (print_errno_n_return(1, "minishell", redir_node->file_path,
				errno));
	if (exe_ctx == RUN_IN_CHILD)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			close_retry(output_fd);
			return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
		}
		if (close_retry(output_fd) == -1)
			return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
		return (execute(redir_node->exe_child, RUN_IN_CHILD, sh_ctx));
	}
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
	{
		close_retry(output_fd);
		return (print_errno_n_return(EXIT_FAILURE, "dup", NULL, errno));
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		close_retry(output_fd);
		close_retry(original_stdout);
		return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
	}
	if (close_retry(output_fd) == -1)
	{
		restore_fd(original_stdout, STDOUT_FILENO);
		return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
	}
	status = execute(redir_node->exe_child, exe_ctx, sh_ctx);
	if (restore_fd(original_stdout, STDOUT_FILENO) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (status);
}

int	execute_redirect_append_output(t_ast_redirection *redir_node,
		t_exec_context exe_ctx, t_shell_context *sh_ctx)
{
	int	append_fd;
	int	status;
	int	original_stdout;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	append_fd = open(redir_node->file_path, flags, 0644);
	if (append_fd == -1)
		return (print_errno_n_return(1, "minishell", redir_node->file_path,
				errno));
	if (exe_ctx == RUN_IN_CHILD)
	{
		if (dup2(append_fd, STDOUT_FILENO) == -1)
		{
			close_retry(append_fd);
			return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
		}
		if (close_retry(append_fd) == -1)
			return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
		return (execute(redir_node->exe_child, RUN_IN_CHILD, sh_ctx));
	}
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
	{
		close_retry(append_fd);
		return (print_errno_n_return(EXIT_FAILURE, "dup", NULL, errno));
	}
	if (dup2(append_fd, STDOUT_FILENO) == -1)
	{
		close_retry(append_fd);
		close_retry(original_stdout);
		return (print_errno_n_return(EXIT_FAILURE, "dup2", NULL, errno));
	}
	if (close_retry(append_fd) == -1)
	{
		restore_fd(original_stdout, STDOUT_FILENO);
		return (print_errno_n_return(EXIT_FAILURE, "close", NULL, errno));
	}
	status = execute(redir_node->exe_child, exe_ctx, sh_ctx);
	if (restore_fd(original_stdout, STDOUT_FILENO) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (status);
}

int	execute_redirection(t_ast *node, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	int					status;
	t_ast_redirection	*redir_node;

	redir_node = &node->u_data.redirection;
	if (should_fork_on_redirection(redir_node, exe_ctx))
		return (fork_and_run_cmd_in_child(node, sh_ctx));
	status = 1;
	if (redir_node->redir_type == REDIR_INPUT
		|| redir_node->redir_type == REDIR_HEREDOC)
		status = exe_redirect_input(redir_node, exe_ctx, sh_ctx);
	else if (redir_node->redir_type == REDIR_OUTPUT)
		status = exe_redirect_output(redir_node, exe_ctx, sh_ctx);
	else if (redir_node->redir_type == REDIR_APPEND)
		status = execute_redirect_append_output(redir_node, exe_ctx, sh_ctx);
	return (status);
}

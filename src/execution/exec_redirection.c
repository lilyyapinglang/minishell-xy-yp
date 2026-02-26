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

int	exe_redirect_input(t_ast_redirection *redir_node, t_shell_context *sh_ctx)
{
	int	input_fd;
	int	status;
	int	original_stdin;

	input_fd = open(redir_node->file_path, O_RDONLY);
	if (input_fd == -1)
		return (print_errno_n_return(1, "minishell", redir_node->file_path,
				errno));
	original_stdin = dup(STDIN_FILENO);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	status = execute(redir_node->exe_child, RUN_IN_SHELL, sh_ctx);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

int	exe_redirect_output(t_ast_redirection *redir_node, t_shell_context *sh_ctx)
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
	original_stdout = dup(STDOUT_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
	status = execute(redir_node->exe_child, RUN_IN_SHELL, sh_ctx);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	return (status);
}

int	execute_redirect_append_output(t_ast_redirection *redir_node,
		t_shell_context *sh_ctx)
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
	original_stdout = dup(STDOUT_FILENO);
	dup2(append_fd, STDOUT_FILENO);
	close(append_fd);
	status = execute(redir_node->exe_child, RUN_IN_SHELL, sh_ctx);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	return (status);
}

int	execute_redirection(t_ast *node, t_shell_context *sh_ctx)
{
	int					status;
	t_ast_redirection	*redir_node;

	redir_node = &node->u_data.redirection;
	status = 1;
	if (redir_node->redir_type == REDIR_INPUT
		|| redir_node->redir_type == REDIR_HEREDOC)
		status = exe_redirect_input(redir_node, sh_ctx);
	else if (redir_node->redir_type == REDIR_OUTPUT)
		status = exe_redirect_output(redir_node, sh_ctx);
	else if (redir_node->redir_type == REDIR_APPEND)
		status = execute_redirect_append_output(redir_node, sh_ctx);
	return (status);
}

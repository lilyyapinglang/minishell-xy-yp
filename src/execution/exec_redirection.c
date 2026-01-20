// int		apply_input_redir(t_cmd *cmd);

// int		apply_output_redir(t_cmd *cmd);

// void	apply_all_redirs(t_cmd *cmd);
#include "../inc/minishell.h"

/*
cat < input.txt
redirection: let stdin point to input.txt
child : cat
//child ──> AST_COMMAND / AST_PIPELINE / AST_SUBSHELL / ...
redirect_input(redir):
	把 stdin 改成 file
	执行 redir->child
	把 stdin 改回原样
redirection 节点的职责是：

建立环境

调用 child

回收环境
*/
int	exe_redirect_input(t_ast_redirection *redir_node, t_shell_context *sh_ctx)
{
	int	input_fd;
	int	status;
	int	original_stdin;

	// fd point to the input file or temp file, fd =3 , 4 5..
	// now i have a file pipe, not not yet stdin (fd=0)
	input_fd = open(redir_node->file_path, O_RDONLY);
	if (input_fd == -1)
		// need to return detailed error info too. eg bash: input.txt: No such file or directory
		return (print_errno_n_return(1, "minishell", redir_node->file_path,
				errno));
	// let orignal_stdii point to the current stdin,
	//		which is terminal input, because we are doing to chagne stdin,
	// so have to save it becasue we need tp return to terminal prompt.
	original_stdin = dup(STDIN_FILENO);
	// link stdin behavior to inputfile
	dup2(input_fd, STDIN_FILENO);
	// after dup2, fd=0 already point to file, input_fd becomes a useless alias
	close(input_fd);
	status = execute(redir_node->exe_child, RUN_IN_SHELL, sh_ctx);
	// after executing child, need to change stdin back to terminal
	// use backup orignal_stdio to link stdin(0) to original places
	dup2(original_stdin, STDIN_FILENO);
	// after recover to terminal, close the temp fd
	close(original_stdin);
	return (status);
}

int	exe_redirect_output(t_ast_redirection *redir_node, t_shell_context *sh_ctx)
{
	int	output_fd;
	int	status;
	int	original_stdout;

	output_fd = open(redir_node->file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
		// need to return detailed error info too. eg bash: input.txt: No such file or directory
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

	append_fd = open(redir_node->file_path, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (append_fd == -1)
		return (1); // need to return detailed error info too
	original_stdout = dup(STDOUT_FILENO);
	dup2(append_fd, 0);
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

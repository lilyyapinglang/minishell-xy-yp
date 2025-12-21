#include "../inc/minishell.h"

// execute as dispatcher
/*
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR

*/
int	exec_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);

	if (node->type == AST_LOGICAL)
		return (exec_logical(node, shell));
	else if (node->type == AST_PIPELINE)
		return (exec_pipeline(node, shell));

	else if (node->type == AST_SUBSHELL)
		return (exec_subshell(node, shell));

	else if (node->type == AST_COMMAND)
		return (exec_cmd(node->cmd, shell));
}
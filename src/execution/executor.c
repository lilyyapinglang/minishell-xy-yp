#include "../inc/minishell.h"

// execute as dispatcher, recursively call execute
/*
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR

*/
int	execute(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);

	if (node->type == AST_LOGICAL)
		return (execute_logical(node, shell));
	else if (node->type == AST_PIPELINE)
		return (execute_pipeline(node, shell));

	else if (node->type == AST_SUBSHELL)
		return (execute_subshell(node, shell));

	else if (node->type == AST_COMMAND)
		return (execute_cmd(node->cmd, shell));
}
#include "../inc/minishell.h"

/*
此时 execute_logical 的策略（概念上）是：
先执行 OR 的 left（也就是 AND 子树），拿到 status_left
如果 status_left == 0：短路成功，不执行 right
否则执行 OR 的 right，返回其 status

*/

int	execute_logical(t_ast_logical *logical_node, t_shell shell)
{
	int	left_status;
	int	right_status;

	left_status = execute(logical_node->left, RUN_IN_SHELL, shell);
	shell.last_status = left_status;
	if (logical_node->operator= TK_AND)
	{
		// execute left first
		if (left_status != 0)
			return (1);
		// left exit with 0; try execute right too
		if (left_status == 0)
		{
			right_status = execute(logical_node->right, RUN_IN_SHELL, shell);
			if (right_status == 0)
				return (0);
			else
				return (1);
		}
	}
	else if (logical_node.operator= TK_OR)
	{
		// execute left first
		if (left_status == 0)
			return (0); // no need to execute right
		if (left_status != 0)
		{
			right_status = execute(logical_node->right, RUN_IN_SHELL, shell);
			if (right_status == 0)
				return (0);
			else
				return (1);
		}
	}
}

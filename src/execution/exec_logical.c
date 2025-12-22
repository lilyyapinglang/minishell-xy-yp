#include "../inc/minishell.h"

/*
此时 execute_logical 的策略（概念上）是：
先执行 OR 的 left（也就是 AND 子树），拿到 status_left
如果 status_left == 0：短路成功，不执行 right
否则执行 OR 的 right，返回其 status

*/

int	execute_logical(t_ast_logical *node, t_shell shell)
{
	if (node.operator= '&&')
	{
		// execute left first
		if (execute(node->left, shell) != 0)
			return (1);
		// left exit with 0; try execute right too
		if (execute(node->right, shell) == 0)
			return (1);
		else
			return (0); //
	}
	else if (node.operator= '||')
	{
		// execute left first
		if (execute(node->left, shell) == 0)
			return (0); // no need to execute right
		if (execute(node->right) == 0)
			return (0);
		else
			return (1);
	}
}

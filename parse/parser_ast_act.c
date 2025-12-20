/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_act.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:32:05 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:35:22 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"

t_ast	*ast_new(t_ast_type type)
{
	t_ast	*n;

	n = safe_malloc(sizeof(t_ast));
	if (!n)
		return (NULL);
	n->type = type;
	n->left = NULL;
	n->right = NULL;
	n->cmd = NULL;
	return (n);
}

/* 叶子：命令节点。失败时释放 cmd，避免 leak */
t_ast	*ast_cmd(t_cmd *cmd)
{
	t_ast	*n;

	n = ast_new(AST_CMD);
	if (!n)
	{
		if (cmd)
			cmd_free_one(cmd);
		return (NULL);
	}
	n->cmd = cmd;
	return (n);
}

t_ast	*ast_bin(t_ast_type type, t_ast *left, t_ast *right)
{
	t_ast	*n;

	n = ast_new(type);
	if (!n)
	{
		if (left)
			ast_free(left);
		if (right)
			ast_free(right);
		return (NULL);
	}
	n->left = left;
	n->right = right;
	return (n);
}

t_ast	*ast_subshell(t_ast *inside)
{
	t_ast	*n;

	n = ast_new(AST_SUBSHELL);
	if (!n)
	{
		if (inside)
			ast_free(inside);
		return (NULL);
	}
	n->left = inside;
	return (n);
}

void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	if (node->left)
		ast_free(node->left);
	if (node->right)
		ast_free(node->right);
	if (node->type == AST_CMD)
	{
		if (node->cmd)
			cmd_free_one(node->cmd);
	}
	free(node);
}

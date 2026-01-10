/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build_node.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:19:09 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/10 14:50:35 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parse_error.h"   // only if it uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_ast *create_ast_command(char **argv, t_shell_context *sh)
{
	t_ast *node;

	init_ast_node(&node, AST_COMMAND, sh);
	node->u_data.command.args = argv;
	return (node);
}

t_ast *create_ast_subshell(t_ast *child, t_shell_context *sh)
{
	t_ast *node;

	if (!child)
		return (set_syntax_error("(", sh));
	init_ast_node(&node, AST_SUBSHELL, sh);
	node->u_data.subshell.child = child;
	return (node);
}

t_ast *create_ast_redir(t_token_type direction, t_list *tk_filename,
						t_ast *child, t_shell_context *sh)
{
	t_ast *node;

	if (!tk_filename || tk_type(tk_filename) != TOKEN_WORD)
		return (set_syntax_error(tk_type_to_string(direction), sh));
	init_ast_node(&node, AST_REDIRECTION, sh);
	node->u_data.redirection.redir_type = direction;
	node->u_data.redirection.file_path = tk_value(tk_filename);
	node->u_data.redirection.exe_child = child;
	return (node);
}

t_ast *create_ast_pipeline(t_ast *left, t_ast *right, t_shell_context *sh)
{
	t_ast *node;

	if (!left || !right)
		return (set_syntax_error("|", sh));
	init_ast_node(&node, AST_PIPELINE, sh);
	node->u_data.pipeline.left = left;
	node->u_data.pipeline.right = right;
	return (node);
}

t_ast *create_ast_and_or(t_ast *left, t_token_type operator, t_ast *right,
						 t_shell_context *sh)
{
	t_ast *node;

	if (!left || !right)
		return (set_syntax_error(tk_type_to_string(operator), sh));
	init_ast_node(&node, AST_LOGICAL, sh);
	node->u_data.logical.operator = operator;
	node->u_data.logical.left = left;
	node->u_data.logical.right = right;
	return (node);
}

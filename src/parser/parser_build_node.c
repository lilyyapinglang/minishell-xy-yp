/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build_node.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:19:09 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:15:47 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include "safefunctions.h"

t_ast	*create_ast_command(char **argv, t_shell_context *sh)
{
	t_ast	*node;

	init_ast_node(&node, AST_COMMAND, sh);
	node->u_data.command.args = argv;
	return (node);
}

t_ast	*create_ast_redir(t_token_type direction, t_list *tk_filename,
		t_ast *child, t_shell_context *sh)
{
	t_ast	*node;

	if (!tk_filename || tk_type(tk_filename) != TOKEN_WORD)
		return (set_syntax_error(tk_type_to_string(direction), sh));
	init_ast_node(&node, AST_REDIRECTION, sh);
	node->u_data.redirection.redir_type = direction;
	node->u_data.redirection.file_path = tk_value(tk_filename);
	node->u_data.redirection.exe_child = child;
	return (node);
}

t_ast	*create_ast_pipeline(t_ast *left, t_ast *right, t_shell_context *sh)
{
	t_ast	*node;

	if (!left || !right)
		return (set_syntax_error("|", sh));
	init_ast_node(&node, AST_PIPELINE, sh);
	node->u_data.pipeline.left = left;
	node->u_data.pipeline.right = right;
	return (node);
}

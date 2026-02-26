/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:02:22 by ylang             #+#    #+#             */
/*   Updated: 2026/02/20 18:02:25 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "expander.h"

static int	finalize_status(int status, t_exec_context exe_ctx,
		t_shell_context *sh_ctx)
{
	sh_ctx->last_status = status;
	if (exe_ctx == RUN_IN_CHILD)
		exit(status);
	return (status);
}

// execute as dispatcher, recursively call execute
/*
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR

*/
// until till command level we decide we we want
// this command to return to main shell by return or simply exit

int	execute(t_ast *node, t_exec_context execution_context,
		t_shell_context *sh_ctx)
{
	int	status;

	status = EXIT_SUCCESS;
	if (!node)
		return (finalize_status(0, execution_context, sh_ctx));
	if ((node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		&& node->is_expanded == false)
	{
		expander(node, sh_ctx);
		node->is_expanded = true;
	}
<<<<<<< HEAD
	/*

	typedef struct s_ast_logical
	{
		t_token_type	operator;
		struct s_ast	*left;
		struct s_ast	*right;
	}							t_ast_logical;
	*/
	
	if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh_ctx);

	/*
	typedef struct s_ast_subshell
	{
	struct s_ast		*child;
	}						t_ast_subshell;
	*/
=======
	if (node->type == AST_LOGICAL)
		status = execute_logical(node, sh_ctx);
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh_ctx);
	else if (node->type == AST_SUBSHELL)
		status = execute_subshell(node, sh_ctx);
>>>>>>> main
	else if (node->type == AST_REDIRECTION)
		status = execute_redirection(node, sh_ctx);
	else if (node->type == AST_COMMAND)
		status = execute_command(node, execution_context, sh_ctx);
	else
		return (print_msg_n_return(1, "execute", NULL, "illegal node type"));
	return (finalize_status(status, execution_context, sh_ctx));
}

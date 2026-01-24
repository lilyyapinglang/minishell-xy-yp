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
// Expand node  should come from expander

int	execute(t_ast *node, t_exec_context execution_context,
		t_shell_context *sh_ctx)
{
	int status;
	status = EXIT_SUCCESS;

	if (!node)
		return (finalize_status(0, execution_context, sh_ctx));

	// 1. only ast_redirection and ast_commdn that actully has content *file and **argv to expand
	// 2. Other types logicla, pipeline, subshell are just structure ,
	// they don't have string sto expand
	// 3. as long as an node is going to be executed,
	// it guarranteed it will be expanded before actual exection
	// 4. why not in side exeectuion_redir or execution_cmd ,
	// to gurantee it will be only expanded once
	if ((node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		&& execution_context != RUN_IN_CHILD)
	{
		// if (node->type == AST_COMMAND)
		// 	printf("node->type == AST_COMMDN\n");
		// printf("I entered node->type expander \n");
		expander(node, sh_ctx);
		// printf("haha %s\n", node->u_data.command.args[1]);
	}
	/*

	typedef struct s_ast_logical
	{
		t_token_type	operator;
		struct s_ast	*left;
		struct s_ast	*right;
	}							t_ast_logical;
	*/
	if (node->type == AST_LOGICAL)
		// status = execute_logical(&(node->u_data.logical), execution_context,
		// 		sh_ctx);
		status = execute_logical(node, sh_ctx);
	/*
	typedef struct s_ast_pipeline
	{
	struct s_ast		*left;
	struct s_ast		*right;
	}					t_ast_pipeline;
	*/
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh_ctx);

	/*
	typedef struct s_ast_subshell
	{
	struct s_ast		*child;
	}						t_ast_subshell;
	*/
	else if (node->type == AST_SUBSHELL)
		// status = execute_subshell(&(node->u_data.subshell), sh_ctx);
		status = execute_subshell(node, sh_ctx);

	/*
	typedef struct s_ast_redirection
	{
	t_token_type		direction;
	struct s_ast		*child;
	char				*file;
	}			t_ast_redirection;
	*/
	else if (node->type == AST_REDIRECTION)
		// status = execute_redirection(&(node->u_data.redirection), sh_ctx);
		status = execute_redirection(node, sh_ctx);

	/*
	typedef struct s_ast_command
	{
	char				**args;
	}					t_ast_command;
	*/
	// until till command level we decide we we want this command to return to main shell by return or simply exit
	else if (node->type == AST_COMMAND)
		// status = execute_command(&(node->u_data.command), execution_context,
		// 		sh_ctx);
		status = execute_command(node, execution_context, sh_ctx);
	else
		return (print_msg_n_return(1, "execute", NULL, "illegal node type"));
	// should I exit current exection or should i return to main shell prompt
	// if corrent exection is meanted to be exited, quit this shell
	// if (exit_or_return == EXITAFTEREXE)
	// 	quit_child_shell(status, shell);
	// return (status);
	return (finalize_status(status, execution_context, sh_ctx));
}
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
// Expand node  should come from expander

int	execute(t_ast *node, t_exec_context execution_context,
		t_shell_context *shell_conetext)
{
	int status;
	status = EXIT_SUCCESS;
	if (!node)
		return (status);
	// 1. only ast_redirection and ast_commdn that actully has content *file and **argv to expand
	// 2. Other types logicla, pipeline, subshell are just structure ,
	// they don't have string sto expand
	// 3. as long as an node is going to be executed,
	// it guarranteed it will be expanded before actual exection
	// 4. why not in side exeectuion_redir or execution_cmd ,
	// to gurantee it will be only expanded once
	if (node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		expand_node(node, shell);
	/*

	typedef struct s_ast_logical
	{
		t_token_type	operator;
		struct s_ast	*left;
		struct s_ast	*right;
	}							t_ast_logical;
	*/
	if (node->type == AST_LOGICAL)
		status = execute_logical(&(node->u_data.logical), execution_context,
				shell);

	/*
	typedef struct s_ast_pipeline
	{
	struct s_ast		*left;
	struct s_ast		*right;
	}					t_ast_pipeline;
	*/
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, shell);

	/*
	typedef struct s_ast_subshell
	{
	struct s_ast		*child;
	}						t_ast_subshell;
	*/
	else if (node->type == AST_SUBSHELL)
		status = execute_subshell(&(node->u_data.subshell), shell);

	/*
	typedef struct s_ast_redirection
	{
	t_token_type		direction;
	struct s_ast		*child;
	char				*file;
	}			t_ast_redirection;
	*/
	else if (node->type == AST_REDIRECTION)
		status = execute_redirection(&(node->u_data.redirection), shell);

	/*
	typedef struct s_ast_command
	{
	char				**args;
	}					t_ast_command;
	*/
	// until till command level we decide we we want this command to return to main shell by return or simply exit
	else if (node->type == AST_COMMAND)
		status = execute_cmd(&(node->u_data.command), execution_context, shell);
	else
	{
		ft_printf("execute,illegal node type");
		return (EXIT_FAILURE);
	}
	// should I exit current exection or should i return to main shell prompt
	// if corrent exection is meanted to be exited, quit this shell
	// if (exit_or_return == EXITAFTEREXE)
	// 	quit_child_shell(status, shell);
	return (status);
}
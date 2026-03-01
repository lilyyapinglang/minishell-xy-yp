/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 17:45:08 by lilypad           #+#    #+#             */
/*   Updated: 2026/03/01 11:49:01 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
1. Allocate memory for the generic AST node
2. Set the type tag so we know which union member to access later
3. Initialize the specific union data
4. Set default flags
*/
t_ast	*new_ast_command(char **args)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_COMMAND;
	node->u_data.command.args = args;
	node->is_expanded = false;
	return (node);
}

int	builtin_env(char **argv, t_shell_context *sh_ctx)
{
	t_ast	*cmd_node;
	int		status;

	if (!argv[1])
	{
		print_env_or_export(false, sh_ctx);
		return (EXIT_SUCCESS);
	}
	else
	{
		cmd_node = new_ast_command(&argv[1]);
		if (!cmd_node)
			return (EXIT_FAILURE);
		status = execute_command(cmd_node, RUN_IN_CHILD, sh_ctx);
		free(cmd_node);
		return (status);
	}
}

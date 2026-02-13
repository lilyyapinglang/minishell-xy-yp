/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:06:43 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/13 16:12:50 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "expander.h"
#include "ms_error.h"
#include "parse.h"
#include "safefunctions.h"
#include "shell_context.h"
#include "utils.h"

void	expand_command(t_ast *node, t_shell_context *sh)
{
	char	**args;
	t_list	*expanded_args;

	expanded_args = NULL;
	args = node->u_data.command.args;
	while (*args)
	{
		expand_one_arg(*args, &expanded_args, sh);
		args++;
	}
	node->u_data.command.args = convert_list_to_array(&expanded_args, sh);
}

void	expand_redir(t_ast *node, t_shell_context *sh)
{
	char	*str;
	t_list	*expanded_args;

	if (node->u_data.redirection.redir_type == REDIR_HEREDOC)
		return ;
	expanded_args = NULL;
	str = node->u_data.redirection.file_path;
	expand_one_arg(str, &expanded_args, sh);
	if (ft_lstsize(expanded_args) != 1 || !ft_strcmp(expanded_args->content,
			""))
		report_error("redirection:", NULL, " ambiguous redirect", sh);
	else
		node->u_data.redirection.file_path = expanded_args->content;
}

/*entrance : expand a node of a AST*/
t_ast	*expander(t_ast *node, t_shell_context *sh)
{
	if (node->type == AST_COMMAND)
		expand_command(node, sh);
	else if (node->type == AST_REDIRECTION)
		expand_redir(node, sh);
	return (node);
}

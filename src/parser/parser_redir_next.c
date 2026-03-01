/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_next.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:21:22 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/13 16:13:02 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"   // only if uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_ast	*build_redirected_command(t_ast *prefix, t_ast *suffix,
		t_ast *command)
{
	t_ast	*last;

	if (prefix && suffix)
	{
		last = get_last_redirection_list(prefix);
		last->u_data.redirection.exe_child = suffix;
		last = get_last_redirection_list(suffix);
		last->u_data.redirection.exe_child = command;
		return (prefix);
	}
	if (prefix && !suffix)
	{
		last = get_last_redirection_list(prefix);
		last->u_data.redirection.exe_child = command;
		return (prefix);
	}
	if (!prefix && suffix)
	{
		last = get_last_redirection_list(suffix);
		last->u_data.redirection.exe_child = command;
		return (suffix);
	}
	return (command);
}

t_ast	*get_last_redirection_list(t_ast *node)
{
	while (node && node->type == AST_REDIRECTION
		&& node->u_data.redirection.exe_child
		&& node->u_data.redirection.exe_child->type == AST_REDIRECTION)
		node = node->u_data.redirection.exe_child;
	return (node);
}

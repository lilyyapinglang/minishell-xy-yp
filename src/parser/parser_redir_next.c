/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_next.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:21:22 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/10 14:50:38 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parse_error.h"   // only if it uses set_syntax_error/report_syntax_error
#include "safefunctions.h" // only if it calls calloc_s/s_alloc/track_alloc/etc

t_ast	*build_redirected_command(t_ast *prefix, t_ast *suffix, t_ast *command)
{
	if (prefix && suffix)
	{
		get_last_redirection_list(prefix)->u_data.redirection.exe_child = suffix;
		get_last_redirection_list(suffix)->u_data.redirection.exe_child = command;
		return (prefix);
	}
	else if (prefix && !suffix)
	{
		get_last_redirection_list(prefix)->u_data.redirection.exe_child = command;
		return (prefix);
	}
	else if (!prefix && suffix)
	{
		get_last_redirection_list(suffix)->u_data.redirection.exe_child = command;
		return (suffix);
	}
	else
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

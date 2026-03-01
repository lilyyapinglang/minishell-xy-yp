/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:20:22 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:15:54 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include "safefunctions.h"

t_ast	*parse_redir(t_list **token, t_shell_context *sh)
{
	t_ast	*prefix;
	t_ast	*suffix;
	t_ast	*command;

	prefix = parse_redir_list(token, NULL, sh);
	if (sh->parsing_error)
		return (NULL);
	command = parse_command(token, sh);
	suffix = parse_redir_list(token, command, sh);
	if (sh->parsing_error)
		return (NULL);
	return (build_redirected_command(prefix, suffix, command));
}

bool	parse_new_command_arg(t_list **token, t_ast *command,
		t_shell_context *sh)
{
	if (command && *token && is_word(peek_list(*token)))
	{
		add_arg_to_array(&command->u_data.command.args, tk_value(*token), sh);
		advance_list(token);
		return (true);
	}
	return (false);
}

bool	is_in_sequential_redirection_list(t_list **token, t_ast *command)
{
	if (*token && is_redir(peek_list(*token)))
		return (true);
	if (command && *token && is_word(peek_list(*token)))
		return (true);
	return (false);
}

static void	link_redir_node(t_ast **first, t_ast **last, t_ast *new_node)
{
	if (!*first)
		*first = new_node;
	else
		(*last)->u_data.redirection.exe_child = new_node;
	*last = new_node;
}

t_ast	*parse_redir_list(t_list **token, t_ast *command, t_shell_context *sh)
{
	t_ast			*first;
	t_ast			*last;
	t_ast			*new_node;
	t_token_type	dir;

	first = NULL;
	last = NULL;
	while (is_in_sequential_redirection_list(token, command))
	{
		if (parse_new_command_arg(token, command, sh))
			continue ;
		dir = peek_list(*token);
		advance_list(token);
		if (!*token)
			return (set_syntax_error("\\n", sh), NULL);
		if (!is_word(peek_list(*token)))
			return (set_syntax_error(tk_value(*token), sh), NULL);
		new_node = create_ast_redir(dir, *token, NULL, sh);
		if (!new_node || sh->parsing_error)
			return (NULL);
		link_redir_node(&first, &last, new_node);
		advance_list(token);
	}
	return (first);
}

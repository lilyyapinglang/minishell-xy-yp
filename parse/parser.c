/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 17:33:50 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 22:47:30 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/token.h"
#include "../include/parse.h"

static t_ast *parse_primary(t_parser *parser)
{
	t_ast *node;
	t_ast *tmp;

	if (match(parser, TOKEN_LPAREN))
	{
		node = parse_and_or(parser);
		if (!node)
			return (NULL);
		if (!match(parser, TOKEN_RPAREN))
		{
			parser->error = 1;
			ast_free(node);
			return (NULL);
		}
		tmp = ast_subshell(node);
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	return (parse_simple_command(parser));
}

static t_ast *parse_pipeline(t_parser *parser)
{
	t_ast *left;
	t_ast *right;
	t_ast *tmp;

	left = parse_primary(parser);
	if (!left)
		return (NULL);
	while (match(parser, TOKEN_PIPE))
	{
		right = parse_primary(parser);
		if (!right)
		{
			ast_free(left);
			return (NULL);
		}
		tmp = ast_bin(AST_PIPE, left, right);
		if (!tmp)
			return (NULL);
		left = tmp;
	}
	return (left);
}

static t_ast *parse_and_or(t_parser *parser)
{
	t_ast *left;
	t_ast *right;
	t_ast *tmp;
	t_token_type op;

	left = parse_pipeline(parser);//take left
	if (!left)
		return (NULL);
	while (peek(parser) == TOKEN_AND || peek(parser) == TOKEN_OR)//keep detecting the AND and OR
	{
		op = peek(parser);
		advance(parser);
		right = parse_pipeline(parser);//take right
		if (!right)
		{
			ast_free(left);
			return (NULL);
		}
		if (op == TOKEN_AND)
			tmp = ast_bin(AST_AND, left, right);//add left and right together, the ori tree is the type
		else
			tmp = ast_bin(AST_OR, left, right);
		if (!tmp)
			return (NULL);
		left = tmp;//make the resutl as left, keep detecting other && or ||s
	}
	return (left);
}

static t_ast *parse_simple_command(t_parser *parser)
{
	t_cmd *cmd;
	t_ast *node;

	cmd = cmd_create();
	if (!cmd)
		return (NULL);
	while (parser->current && !is_stop_token(parser->current->type))
	{
		if (parser->current->type >= REDIR_INPUT && parser->current->type <= REDIR_HEREDOC)
		{
			if (!parse_redir(parser, cmd))
			{
				cmd_free_one(cmd);
				return (NULL);
			}
		}
		else if (parser->current->type == TOKEN_WORD)
		{
			if (!parse_arg(cmd, parser->current->value))
			{
				cmd_free_one(cmd);
				return (NULL);
			}
			advance(parser);
		}
		else
			break;
	}
	if (!cmd->argv && !cmd->redirects)
	{
		cmd_free_one(cmd);
		return (NULL);
	}
	node = ast_cmd(cmd);
	if (!node)
		return (NULL);
	return (node);
}

/* entrance */

t_ast *parser(t_tokenlist *tokens)
{
	t_parser parser;
	t_ast *root;

	if (!tokens)
		return (NULL);
	if (!parser_check_syntax(tokens))
		return (NULL);
	parser.current = tokens;
	parser.error = 0;
	root = parse_and_or(&parser);
	if (parser.error || !root)
	{
		ast_free(root);
		return (NULL);
	}
	if (peek(&parser) != TOKEN_EOF) // if everything is legal it should com to the end
	{
		ast_free(root);
		return (NULL);
	}
	return (root);
}

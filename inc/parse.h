/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:22:47 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/09 02:27:28 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
#define PARSE_H

#include "../inc/minishellparse.h"
/*token part*/
/*
typedef enum e_token_type
{
	TOKEN_WORD,

	TOKEN_LPAREN, //  (
	TOKEN_RPAREN, //  )

	TOKEN_PIPE, // |
	TOKEN_AND,	// &&
	TOKEN_OR,	// ||

	REDIR_INPUT,   // <
	REDIR_OUTPUT,  // >
	REDIR_APPEND,  // >>
	REDIR_HEREDOC, // <<

	TOKEN_EOF,
	TOKEN_INVALID // sth like illegal &
} t_token_type;

typedef enum e_quote_state
{
	QUOTE_NONE, // not in the quote status
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_state;

chain of the tokens use list in minishell.h

typedef struct s_token
{
	t_token_type type;
	char *value;
} t_token;

// ----- PARSER ----- //
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR
} t_ast_type;

typedef struct s_ast_command
{
	char **args;
} t_ast_command;

typedef struct s_ast_pipeline
{
	struct s_ast *left;
	struct s_ast *right;
} t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type operator;
	struct s_ast *left;
	struct s_ast *right;
} t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type redir_type;
	// child: the AST executed under this redirection's file-descriptor environment
	struct s_ast *exe_child;
	char *file_path;
} t_ast_redirection;

typedef struct s_ast_subshell
{
	struct s_ast *child;
} t_ast_subshell;

typedef struct s_ast_syntax_error
{
	char *unexpected_token;
} t_ast_syntax_error;

typedef struct s_ast
{
	t_ast_type type;
	union
	{
		t_ast_command command;
		t_ast_pipeline pipeline;
		t_ast_logical logical;
		t_ast_redirection redirection;
		t_ast_subshell subshell;
		t_ast_syntax_error s_error;
	} u_data;
} t_ast;
*/

#endif
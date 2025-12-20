/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:22:47 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/20 21:47:49 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
#define PARSE_H

#include "../include/token.h"
/*
typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;
*/

typedef struct s_redir
{
	t_token_type type;
	char *target;
	int heredoc_fd;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char **argv; // is a list , we put every cmd in it
	t_redir *redirects;
	//struct s_cmd *next; // cmd1 -> cmd2 -> cmd3 -> NULL
	int pipe_fd[2];
} t_cmd;

/*AST NODE TYPE*/
typedef enum e_ast_type
{
	AST_CMD,	 // simple cmd with argv / redirects
	AST_PIPE,	 // child tree
	AST_AND,	 // &&
	AST_OR,		 // ||
	AST_SUBSHELL // ( ... )
} t_ast_type;

typedef struct s_ast
{
	t_ast_type type;
	struct s_ast *left;
	struct s_ast *right;
	t_cmd *cmd; // only for ast_cmd
} t_ast;

/*parser status*/

typedef struct s_parser
{
	t_tokenlist *current;
	int error;
} t_parser;

/* PARSER ENTRY  */
t_ast	*parser(t_tokenlist *tokens);

/*  BASIC ACTIONS  */
t_token_type	peek(t_parser *p);
void			advance(t_parser *p);
int				match(t_parser *p, t_token_type type);

/*   AST BUILD  */
t_ast	*ast_new(t_ast_type type);
t_ast	*ast_cmd(t_cmd *cmd);
t_ast	*ast_bin(t_ast_type type, t_ast *left, t_ast *right);
t_ast	*ast_subshell(t_ast *inside);
void	ast_free(t_ast *node);

/*   CMD / ARGV  */
t_cmd	*cmd_create(void);
void	cmd_free_one(t_cmd *cmd);
int		parse_arg(t_cmd *cmd, char *arg);

/*   REDIRECTION  */
t_redir	*redir_create(t_token_type type, char *target);
void	redir_add_back(t_redir **list, t_redir *new);
void	redir_list_free(t_redir *redirects);
int		parse_redir(t_parser *parser, t_cmd *cmd);

/*   UTIL  */
int		is_stop_token(t_token_type type);

/*   SYNTAX CHECK   */
int		parser_check_syntax(t_tokenlist *tokens);

/*   SYNTAX CHECK  SUB */
int	is_redir(t_token_type t);
int	is_binary_op(t_token_type t);
int	is_wrong_start(t_token_type t);
int	is_wrong_after_op(t_token_type t);

int	syntax_error_token(t_tokenlist *tok);

int	check_redirect(t_tokenlist *cur);
int	check_binary_op(t_tokenlist *cur);
int	check_lparen(t_tokenlist *cur);
int	check_rparen(t_tokenlist *prev, t_tokenlist *cur);

int	check_one_token(t_tokenlist *prev, t_tokenlist *cur, int *parens);
int	check_end_token(t_tokenlist *last, int parens);
#endif
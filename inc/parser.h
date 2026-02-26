/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:12:32 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 20:12:33 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "parse.h"
# include "list.h"
#include "shell_context.h"


int					parser(t_list *token_list, t_ast **ast,
						t_shell_context *sh);

/*parse_sub*/
t_ast				*parse_and_or(t_list **token, t_shell_context *sh);
t_ast				*parse_pipeline(t_list **token, t_shell_context *sh);
t_ast				*parse_subshell(t_list **token, t_shell_context *sh);
t_ast				*parse_command(t_list **token, t_shell_context *sh);

/*parser_tk_type*/
t_token_type		tk_type(t_list *token);
char				*tk_value(t_list *token);
int					is_and_or(t_token_type t);
int					is_word(t_token_type t);
int					is_redir(t_token_type t);

/*parser_redir*/
t_ast				*parse_redir(t_list **token, t_shell_context *sh);
bool				parse_new_command_arg(t_list **token, t_ast *command,
						t_shell_context *sh);
bool				is_in_sequential_redirection_list(t_list **token,
						t_ast *command);
t_ast				*parse_redir_list(t_list **token, t_ast *command,
						t_shell_context *sh);

/*parser_redir_next*/
t_ast				*build_redirected_command(t_ast *prefix, t_ast *suffix,
						t_ast *command);
t_ast				*get_last_redirection_list(t_ast *node);

/*parser_build_node*/
t_ast				*create_ast_command(char **argv, t_shell_context *sh);
t_ast				*create_ast_subshell(t_ast *child, t_shell_context *sh);
t_ast				*create_ast_redir(t_token_type direction,
						t_list *tk_filename, t_ast *child, t_shell_context *sh);
t_ast				*create_ast_pipeline(t_ast *left, t_ast *right,
						t_shell_context *sh);
t_ast				*create_ast_and_or(t_ast *left, t_token_type operator, t_ast
						* right, t_shell_context *sh);

/*basic actions*/
t_token_type		peek_list(t_list *tok);
void				advance_list(t_list **tok);
int					match_list(t_list **tok, t_token_type type);

/*parser_utile*/
void				init_ast_node(t_ast **node, t_ast_type type,
						t_shell_context *sh);
char				*tk_type_to_string(t_token_type type);
#endif
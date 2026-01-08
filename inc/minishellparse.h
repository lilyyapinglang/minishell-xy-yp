/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellparse.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:13:39 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 19:34:49 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef MINISHELLPARSE_H
#define MINISHELLPARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>
#include <stdbool.h>
#include <stddef.h>

#include "../libft/libft.h"
#include "parse.h"
#include "safefunctions.h"

#define PROMPT "minishell$ "
#define ERROR_PROMPT "minishell: "

typedef struct s_list
{
	void *content;		 // user-owned data
	struct s_list *next; // next node
	struct s_list *prev; // previous node
} t_list;

typedef struct s_shell_context
{
	/* ---------- Environment ---------- */
	t_list *env; // environment variables (KEY=VALUE)
	// char *home;  // cached $HOME value
	/* ---------- Memory / Resource Tracking ---------- */
	t_list *allocated_pointers[3];
	//// categorized allocation tracking (lifetime-based cleanup)
	t_list *temporary_files;
	// heredoc files or other temp resources to unlink on exit
	/* ---------- Parsing / Execution State ---------- */
	char *parsing_error; // error message produced during parsing
	bool in_main_process;
	// true: interactive shell process
	// false: forked child (pipeline stage / subshell)
	int last_status;
	// exit status of the last executed command ($?)
} t_shell_context;



/*token part*/

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

/*chain of the tokens use list in minishell.h*/

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


/*expander*/


typedef enum e_exp_context
{
	NO_QUOTE,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE
}							t_exp_context;

typedef struct s_expander
{
	t_exp_context			context;
	int						i;
	char					*buf;
	int						buf_size;
	int						buf_i;
	t_list					**tokens;
	bool					empty_quotes;
	t_list					*wildcards_position;
}							t_expander;

typedef enum e_tracking_scope
{
	ONLY_CHECK,
	SH,
	PROMPT_S
}							t_tracking_scope;


/********lexer functions********/
int lexer(char *input, t_list **token_list, t_shell_context *sh);
t_token *create_token(t_token_type type, char *value, size_t len, t_shell_context *sh);

/*lexer_utile*/
int ft_isspace(int c);
int ft_istoken(int c);

/*lexer_single token*/
t_token_type scan_next_token(char *input, size_t *len, t_shell_context *sh);
t_token_type token_subshell(char *input, size_t *len);
t_token_type token_and_or_pipe(const char *input, char c, size_t *len,
							   t_shell_context *sh);
t_token_type token_redir(char *input, char c, size_t *len, t_shell_context *sh);
t_token_type token_word(char *input, size_t *len, t_shell_context *sh);

/*!!!!only for testting!!!*/
//const char *get_token_type_name(t_token_type type);
//void display_tokens(t_list *tokens);
//void print_token_list(t_list *tokens);
//void run_test(const char *input);
//int main(int argc, char **argv);

/********parsing functions********/

int parser(t_list *token_list, t_ast **ast, t_shell_context *sh);

/*parse_sub*/
t_ast *parse_and_or(t_list **token, t_shell_context *sh);
t_ast *parse_pipeline(t_list **token, t_shell_context *sh);
t_ast *parse_subshell(t_list **token, t_shell_context *sh);
t_ast *parse_command(t_list **token, t_shell_context *sh);

/*parser_tk_type*/
t_token_type tk_type(t_list *token);
char *tk_value(t_list *token);
int is_and_or(t_token_type t);
int is_word(t_token_type t);
int is_redir(t_token_type t);

/*parser_redir*/
t_ast *parse_redir(t_list **token, t_shell_context *sh);
bool parse_new_command_arg(t_list **token, t_ast *command, t_shell_context *sh);
bool is_in_sequential_redirection_list(t_list **token, t_ast *command);
t_ast *parse_redir_list(t_list **token, t_ast *command, t_shell_context *sh);

/*parser_redir_next*/
t_ast *build_redirected_command(t_ast *prefix, t_ast *suffix, t_ast *command);
t_ast *get_last_redirection_list(t_ast *node);

/*parser_build_node*/
t_ast *create_ast_command(char **argv, t_shell_context *sh);
t_ast *create_ast_subshell(t_ast *child, t_shell_context *sh);
t_ast *create_ast_redir(t_token_type direction, t_list *tk_filename,
						t_ast *child, t_shell_context *sh);
t_ast *create_ast_pipeline(t_ast *left, t_ast *right, t_shell_context *sh);
t_ast *create_ast_and_or(t_ast *left, t_token_type operator, t_ast * right,
						 t_shell_context *sh);

/*basic actions*/
t_token_type peek_list(t_list *tok);
void advance_list(t_list **tok);
int match_list(t_list **tok, t_token_type type);

/*parser_utile*/
void init_ast_node(t_ast **node, t_ast_type type, t_shell_context *sh);
char *tk_type_to_string(t_token_type type);

/*parser_test.c*/

int main(int argc, char **argv);
/*******safe + error functions  */

/*safe_list*/
void lst_add_front_s(void *content, t_list **lst, t_tracking_scope scope,
                     t_shell_context *sh);
void lst_add_back_s(void *content, t_list **lst, t_tracking_scope scope,
                    t_shell_context *sh);
void check_node_alloc(t_list *new_node, void *pointer, t_shell_context *sh);
void remove_list_node(t_list **node, t_list **head,
                      void (*free_function)(void *), bool free_node);
int count_strs(char **argv);

/*libft_list*/
void ft_lstadd_back(t_list **lst, t_list *n);
void ft_lstadd_front(t_list **lst, t_list *n);
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstlast(t_list *lst);
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list *ft_lstnew(void *content);
int ft_lstsize(t_list *lst);
void add_arg_to_array(char ***array, char *new_arg, t_shell_context *sh);

/*safe alloc*/
void *s_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);
void *calloc_s(size_t count, size_t size, t_tracking_scope scope,
               t_shell_context *sh);
void track_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);

/*error*/
ssize_t ft_write_fd(const char *s, int fd);
void *set_syntax_error(char *unexpected_token, t_shell_context *sh);
int report_syntax_error(t_shell_context *sh);
int report_error(char *context, char *element, char *description, t_shell_context *sh);
ssize_t write_s(const char *s, int fd, t_shell_context *sh);
void syscall_error(const char *context, int errnum, t_shell_context *sh);

/*quiteshell.c*/
void quit_shell(int exit_status, t_shell_context *sh);
void error(const char *context, char *description, int exit_status,
           t_shell_context *sh);

/*****************safe functions *******************/
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_front(t_list **lst, t_list *n);
void	ft_lstadd_back(t_list **lst, t_list *n);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/*safe alloc*/

void	*s_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);
void	*calloc_s(size_t count, size_t size,
			t_tracking_scope scope, t_shell_context *sh);
void	track_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);


/*safe list*/
void	lst_add_front_s(void *content, t_list **lst,
			t_tracking_scope scope, t_shell_context *sh);
void	lst_add_back_s(void *content, t_list **lst,
			t_tracking_scope scope, t_shell_context *sh);
void	check_node_alloc(t_list *new_node, void *pointer, t_shell_context *sh);
void	remove_list_node(t_list **node, t_list **head,
			void (*free_function)(void *), bool free_node);
#endif
#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution
# include "../lib/ft_printf/ft_printf.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern unsigned int			g_lastcmd_exit_code;

/**
 * Doubly linked list node.
 *
 * Generic container used throughout the shell to store:
 * - environment variables
 * - pipeline command lists
 * - allocated resource tracking
 *
 * `content` ownership depends on the usage context.
 */
typedef struct s_list
{
	void *content;       // user-owned data
	struct s_list *next; // next node
	struct s_list *prev; // previous node
}							t_list;
// ----- CORE ----- //
/**
 * Shell runtime context.
 *
 * Holds all mutable state required during the shell execution:
 * - environment variables
 * - execution context (parent / child process)
 * - last command exit status
 * - resource tracking for safe cleanup
 *
 * This structure is shared across parsing, execution, and signal handling.
 */

typedef struct s_shell_context
{
	/* ---------- Environment ---------- */
	t_list *env; // environment variables (KEY=VALUE)
	char *home;  // cached $HOME value
	/* ---------- Memory / Resource Tracking ---------- */
	t_list					*allocated_pointers[3];
	//// categorized allocation tracking (lifetime-based cleanup)
	t_list					*temporary_files;
	// heredoc files or other temp resources to unlink on exit
	/* ---------- Parsing / Execution State ---------- */
	char *parsing_error; // error message produced during parsing
	bool					in_main_process;
	// true: interactive shell process
	// false: forked child (pipeline stage / subshell)
	int						last_status;
	// exit status of the last executed command ($?)
}							t_shell_context;

/**
 * Allocation tracking group.
 *
 * TRACK_NONE:
 *   Allocation is NOT added to allocated_pointers[]. owned elsewhere
 *   Caller (or another owner container like env list) must free it manually.
 *
 * TRACK_SHELL:
 *   Freed once on quit_shell().
 *
 * TRACK_CYCLE:
 *   Freed at the end of each prompt/command loop (clear_prompt()).
 */

typedef enum e_tracking_scope
{
	TRACK_NONE,  // not tracked; freed manually
	TRACK_SHELL, // lifetime: whole shell
	TRACK_CYCLE  // lifetime: one REPL iteration / one command line
}							t_tracking_scope;

/*
typedef enum e_redir_type
{
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_APPEND,  //>>
	REDIR_HEREDOC, //<<
}							t_redir_type;
							*/
// core

// ----- LEXER ----- //
typedef enum e_token_type
{
	TK_ERROR,
	TK_WORD,
	TK_PIPE,
	TK_AND,
	TK_OR,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_APPEND_OUT,
	TK_HEREDOC,
	TK_SUBSHELL_OPEN,
	TK_SUBSHELL_CLOSE
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
}							t_token;

typedef struct s_shell
{
	t_env					*env;
	int						last_status;
	bool					in_main_process;
}							t_shell;

// parser //
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR
}							t_ast_type;

typedef struct s_ast_command
{
	char					**args;
}							t_ast_command;

typedef struct s_ast_pipeline
{
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type			operator;
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type			redir_type;
	// child: the AST executed under this redirection's file-descriptor environment
	struct s_ast			*exe_child;
	char					*file_path;
}							t_ast_redirection;

typedef struct s_ast_subshell
{
	struct s_ast			*child;
}							t_ast_subshell;

typedef struct s_ast_syntax_error
{
	char					*unexpected_token;
}							t_ast_syntax_error;

typedef struct s_ast
{
	t_ast_type				type;
	union
	{
		t_ast_command		command;
		t_ast_pipeline		pipeline;
		t_ast_logical		logical;
		t_ast_redirection	redirection;
		t_ast_subshell		subshell;
		t_ast_syntax_error	s_error;
	} u_data;
}							t_ast;

// exection
typedef enum e_exec_context

{
	// EXEC_PARENT: must run in current shell process, do NOT fork, do NOT exit
	// EXEC_FORK  : current shell should fork and wait
	// EXEC_CHILD : already in child process, must not return, must exit

	RUN_IN_SHELL,
	RUN_FORK_WAIT,
	RUN_IN_CHILD
}							t_exec_context;

# define READ_END 0
# define WRITE_END 1

// old
typedef struct s_env
{
	char					*key;
	char					*value;
	char					*key_value;
	struct s_env			*prev;
	struct s_env			*next;
}							t_env;

// new
typedef struct s_env_var
{
	char					*name;
	char					*value;
	bool					exported;
}							t_env_var;

typedef int					(*t_builtin_func)(t_ast_command *,
						t_shell_context *);
typedef struct s_builtin
{
	char					*name;
	t_builtin_func			func;
	bool					stateful;
}							t_builtin;

t_builtin_func				get_builtin_func(const char *name);

// built-in
int							builtin_cd(char **argv, t_env *env);
int							builtin_export(char **argv, t_env *env);
int							builtin_unset(char **argv, t_env *env);
int							builtin_exit(char **argv, t_env *env);
int							builtin_echo(char **argv);
int							builtin_pwd(char **argv);
int							builtin_env(t_env *env);

// execute

t_ast_command				*build_fake_cmd_table_for_tests(void);

// signal

volatile sig_atomic_t		g_latest_signal_status = 0;
void						handle_sigint_in_heredoc_mode(int sig_num);
void						handle_sigint_in_prompt_mode(int sig_num);
void						handle_signal_in_exe_main_process(void);
#endif

//// ----- ENVIRONMENT ----- //
#define DEFAULT_PATH

"/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

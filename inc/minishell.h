#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution
# include "../lib/ft_printf/ft_printf.h"
# include "parse.h"
# include "safefunctions.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define PROMPT "minishell$ "
# define ERROR_PROMPT "minishell: "

// structures

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

typedef struct s_env_var
{
	char				*name;
	char				*value;
	bool				exported;
}						t_env_var;

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

// typedef struct s_shell_context
// {
// 	/* ---------- Environment ---------- */
// 	t_list *env; // environment variables (KEY=VALUE)
// 	// char *home;  // cached $HOME value
// 	/* ---------- Memory / Resource Tracking ---------- */
// 	t_list				*allocated_pointers[3];
// 	//// categorized allocation tracking (lifetime-based cleanup)
// 	t_list				*temporary_files;
// 	// heredoc files or other temp resources to unlink on exit
// 	/* ---------- Parsing / Execution State ---------- */
// 	char *parsing_error; // error message produced during parsing
// 	bool				in_main_process;
// 	// true: interactive shell process
// 	// false: forked child (pipeline stage / subshell)
// 	int					last_status;
// 	// exit status of the last executed command ($?)
// }						t_shell_context;

/**
 * Allocation tracking group.
 *
 * ALLOC_UNTRACKED:
 *   Allocation is NOT added to allocated_pointers[]. owned elsewhere
 *   Caller (or another owner container like env list) must free it manually.
 * 		memory is owned and freed by the data structure (env, etc.)
 *
 * ALLOC_SHELL:
 *   shell-lifetime allocations freed once in quit_shell()
 *
 * ALLOC_PROMPT:
 *   Freed at the end of each prompt/command loop (clear_prompt()).
 *
 * Never track memory that is freed manually.
 */

typedef enum e_tracking_scope
{
	ALLOC_UNTRACKED, // not tracked; freed manually
	ALLOC_SHELL,     // lifetime: whole shell
	ALLOC_PROMPT     // lifetime: one REPL iteration / one command line
}						t_tracking_scope;

typedef enum s_prompt_mode
{
	MAIN_PROMPT,
	HEREDOC_PROMPT
}						t_prompt_mode;

// -----Error

// static void					print_error(const char *cmd, const char *arg,
// const char *msg);
// int							print__errno_n_return(const char *cmd,
// 								const char *arg, int errnum);
// void						fatal_errno_quit(t_shell_context *sh_ctx,
// 								int exit_status, const char *cmd,
// 								const char *arg, int errnum);

// void						fatal_err_msg_quit(t_shell_context *sh_ctx,
// 								int exit_status, const char *cmd,
// 								const char *arg, const char *msg);
// void						warn_errno(const char *cmd, const char *arg,
// 								int errnum);
// int							print_err_msg_n_return(const char *cmd,
// 								const char *arg, const char *msg);
// int							print__errno_n_return(const char *cmd,
// 								const char *arg, int errnum);
// new error
void					print_errno(const char *cmd, const char *arg,
							int errnum);
void					print_msg(const char *cmd, const char *arg,
							const char *msg);
int						print_errno_n_return(int code, const char *cmd,
							const char *arg, int errnum);
int						print_msg_n_return(int code, const char *cmd,
							const char *arg, const char *msg);

void					fatal_errno_shell_quit(t_shell_context *sh_ctx,
							int code, const char *cmd, const char *arg,
							int errnum);
void					fatal_msg_shell_quit(t_shell_context *sh_ctx, int code,
							const char *cmd, const char *arg, int errnum);

void					report_child_termination_signal(status, &new_line,
							sh_ctx);
void					wait_status_to_shell_status(status);

// --list ops

t_list					*ft_lstnew(void *content);
void					ft_lstadd_front(t_list **lst, t_list *new);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstlast(t_list *lst);
void					ft_lstadd_back(t_list **lst, t_list *new);
// void					ft_lstdelone(t_list **lst, void (*del)(void *));
void					ft_lstclear(t_list **lst, void (*del)(void *));

// utils-general

int						ft_strcmp(const char *s1, const char *s2);
ssize_t					ft_write_fd(const char *s, int fd);
void					free_env_var(void *content);
int						check_all_digit(char *str);
int						is_only_n(char *str);
// -----main-----

void					init_shell(t_shell_context *sh_ctx, char **envp);
int						shell_repl_loop(t_shell_context *sh_ctx);
char					*prompt_listener(t_prompt_mode mode);
int						prompt_execution(char *line, t_shell_context *sh_ctx);

void					shell_clear_iteration(t_shell_context *sh_ctx);

void					shell_exit(t_shell_context *sh_ctx, int status);

// -----Execution-----
typedef enum e_exec_context

{
	// EXEC_PARENT: must run in current shell process, do NOT fork, do NOT exit
	// EXEC_FORK  : current shell should fork and wait
	// EXEC_CHILD : already in child process, must not return, must exit

	RUN_IN_SHELL,
	RUN_FORK_WAIT,
	RUN_IN_CHILD
}						t_exec_context;

# define READ_END 0
# define WRITE_END 1

// heredocs
int						collect_all_heredocs(t_ast *root,
							t_shell_context *sh_ctx);

// static int				collect_one_heredoc(t_ast *redir_node,
// 							t_shell_context *sh_ctx);

int						read_heredoc_lines(int fd, const char *delimiter,
							t_shell_context *sh_ctx);
char					*heredoc_delimiter_strip(const char *raw, bool *quoted,
							t_shell_context *sh_ctx);

// executor
int						execute(t_ast *node, t_exec_context execution_context,
							t_shell_context *shell_conetext);
int						execute_logical(t_ast *node, t_shell_context *sh_ctx);
int						execute_command(t_ast *node, t_exec_context exe_ctx,
							t_shell_context *sh_ctx);
int						execute_pipeline(t_ast *pipeline_node,
							t_shell_context *shell_conetext);
int						execute_redirection(t_ast_redirection *redir_node,
							t_shell_context *shell_conetext);
int						execute_subshell(t_ast_subshell *subshell_node,
							t_shell_context *shell_conetext);
// builtin- new
typedef int				(*t_builtin_func)(char **argv, t_shell_context *ctx);
typedef enum e_builtin_id
{
	BI_NONE = 0,
	BI_ECHO,
	BI_PWD,
	BI_ENV,
	BI_CD,
	BI_EXPORT,
	BI_UNSET,
	BI_EXIT
}						t_builtin_id;

typedef struct s_builtin_entry
{
	const char			*name;
	t_builtin_id		id;
	t_builtin_func		func;
}						t_builtin_entry;

// -----built-in

// typedef int					(*t_builtin_func)(t_ast_command *,
// 						t_shell_context *);

typedef struct s_builtin
{
	char				*name;
	t_builtin_func		func;
	bool				stateful;
}						t_builtin;

// t_builtin_func				get_builtin_func(const char *name);
bool					is_buildtin(char *cmd);
bool					is_stateful_builtin(char *cmd);

// int							execute_builtin(t_ast_command *cmd,
// 								t_shell_context *ctx);
// int							execute_external_or_die(t_ast_command *cmd,
// 								t_shell_context *ctx);

bool					is_builtin(char *cmd);

int						builtin_cd(char **argv, t_shell_context *sh_ctx);
int						builtin_export(char **argv, t_shell_context *sh_ctx);
int						builtin_unset(char **argv, t_shell_context *sh_ctx);
int						builtin_exit(char **argv, t_shell_context *sh_ctx);
int						builtin_echo(char **argv, t_shell_context *sh_ctx);
int						builtin_pwd(char **argv, t_shell_context *ctx);
int						builtin_env(char **argv, t_shell_context *ctx);
int						execute_builtin(t_ast_command *cmd,
							t_shell_context *ctx);

//-----  signal-----

volatile sig_atomic_t	g_latest_signal_status = 0;
int						set_signal_handler(int sig_num,
							void (*signal_handler)(int), int flags);
void					handle_sigint_in_heredoc_mode(int sig_num);
void					handle_sigint_in_prompt_mode(int sig_num);
void					set_signal_in_main_prompt_mode(void);
void					set_signal_in_heredoc_prompt_mode(void);
void					set_signal_in_exe_main_process(void);
void					set_signal_in_exe_child_process(void);

//// ----- ENVIRONMENT ----- //

t_list					*init_env(char **envp, t_shell_context *sh_ctx);
void					print_env(bool export_format, t_shell_context *sh_ctx);
char					**build_envp_from_env_list(t_shell_context *sh_ctx);
void					add_new_env_var(t_list **env_list, const char *name,
							const char *value, bool exported,
							t_shell_context *sh_ctx);

t_list					*env_node_find(t_list *env, const char *name);
char					*env_get_value(t_list *env, const char *name);
int						env_set_value(t_shell_context *sh_ctx, const char *name,
							const char *value, bool exported);
int						env_append_value(t_shell_context *sh_ctx,
							const char *name, const char *append_str,
							bool exported);
int						env_unset(t_shell_context *sh_ctx, const char *name);
int						env_mark_exported(t_shell_context *ctx,
							const char *name);
t_env_var				*env_var_from_node(t_list *node);

//---test
t_ast_command			*build_fake_cmd_table_for_tests(void);
# define DEFAULT_PATH \
	"/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

#endif
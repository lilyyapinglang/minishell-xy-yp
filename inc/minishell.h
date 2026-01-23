#ifndef MINISHELL_H
# define MINISHELL_H

// define a comman table as a result of parsing and as a execution plan for execution
# include "../lib/libft/libft.h"
# include "env.h"
# include "lexer.h"
# include "list.h"
# include "parse.h"
# include "parser.h"
# include "safefunctions.h"
# include "shell_context.h"
# include "tracking.h"
# include "utils.h"
# include <errno.h>
# include <fcntl.h>
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

# define M_PROMPT "minishell$ "
# define ERROR_PROMPT "minishell: "

/*-----Error-----*/

void							print_errno(const char *cmd, const char *arg,
									int errnum);
void							print_msg(const char *cmd, const char *arg,
									const char *msg);
int								print_errno_n_return(int code, const char *cmd,
									const char *arg, int errnum);
int								print_msg_n_return(int code, const char *cmd,
									const char *arg, const char *msg);

void							fatal_errno_shell_quit(t_shell_context *sh_ctx,
									int code, const char *cmd, const char *arg,
									int errnum);
void							fatal_msg_shell_quit(t_shell_context *sh_ctx,
									int code, const char *cmd, const char *arg,
									const char *msg);

void							report_child_termination_signal(int wait_status,
									const char *cmd_name, t_shell_context *ctx);

int								wait_status_to_shell_status(int wait_status);

// -----main-----

void							init_shell(t_shell_context *sh_ctx,
									char **envp);
int								shell_repl_loop(t_shell_context *sh_ctx);
char							*prompt_listener(t_prompt_mode mode);
int								prompt_execution(char *line,
									t_shell_context *sh_ctx);

void							shell_clear_iteration(t_shell_context *sh_ctx);

void							shell_exit(t_shell_context *sh_ctx, int status);

// -----Execution-----
typedef enum e_exec_context

{
	// EXEC_PARENT: must run in current shell process, do NOT fork, do NOT exit
	// EXEC_FORK  : current shell should fork and wait
	// EXEC_CHILD : already in child process, must not return, must exit

	RUN_IN_SHELL,
	RUN_FORK_WAIT,
	RUN_IN_CHILD
}								t_exec_context;

# define READ_END 0
# define WRITE_END 1

// heredocs
int								collect_all_heredocs(t_ast *root,
									t_shell_context *sh_ctx);

int								read_heredoc_lines(int fd,
									const char *delimiter,
									t_shell_context *sh_ctx);
// char							*heredoc_delimiter_strip(const char *raw,
// 									bool *quoted, t_shell_context *sh_ctx);
char							*heredoc_delimiter_strip(const char *raw,
									bool *quoted);

// executor
int								execute(t_ast *node,
									t_exec_context execution_context,
									t_shell_context *shell_conetext);
int								execute_logical(t_ast *node,
									t_shell_context *sh_ctx);
int								execute_command(t_ast *node,
									t_exec_context exe_ctx,
									t_shell_context *sh_ctx);
int								execute_pipeline(t_ast *pipeline_node,
									t_shell_context *shell_conetext);
int								execute_redirection(t_ast *node,
									t_shell_context *sh_ctx);
int								execute_subshell(t_ast *node,
									t_shell_context *shell_conetext);

// builtin- new
typedef int						(*t_builtin_func)(char **argv,
							t_shell_context *ctx);
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
}								t_builtin_id;

typedef struct s_builtin_entry
{
	const char					*name;
	t_builtin_id				id;
	t_builtin_func				func;
}								t_builtin_entry;

// -----built-in

typedef struct s_builtin
{
	char						*name;
	t_builtin_func				func;
	bool						stateful;
}								t_builtin;

bool							is_buildtin(char *cmd);
bool							is_stateful_builtin(char *cmd);

bool							is_builtin(char *cmd);

int								builtin_cd(char **argv,
									t_shell_context *sh_ctx);
int								builtin_export(char **argv,
									t_shell_context *sh_ctx);
int								builtin_unset(char **argv,
									t_shell_context *sh_ctx);
int								builtin_exit(char **argv,
									t_shell_context *sh_ctx);
int								builtin_echo(char **argv,
									t_shell_context *sh_ctx);
int								builtin_pwd(char **argv, t_shell_context *ctx);
int								builtin_env(char **argv, t_shell_context *ctx);
int								execute_builtin(t_ast_command *cmd,
									t_shell_context *ctx);

//-----  signal-----

extern volatile sig_atomic_t	g_latest_signal_status;
int								set_signal_handler(int sig_num,
									void (*signal_handler)(int), int flags);
void							handle_sigint_in_heredoc_mode(int sig_num);
void							handle_sigint_in_prompt_mode(int sig_num);
void							set_signal_in_main_prompt_mode(void);
void							set_signal_in_heredoc_prompt_mode(void);
void							set_signal_in_exe_main_process(void);
void							set_signal_in_exe_child_process(void);

//---test
t_ast_command					*build_fake_cmd_table_for_tests(void);

#endif
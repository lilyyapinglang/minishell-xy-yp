#ifndef SHELL_CONTEXT_H
# define SHELL_CONTEXT_H

# include <stdbool.h>
# include "list.h"
# include "tracking.h"

typedef struct s_env_var
{
    char *name;
    char *value;
    bool exported;
} t_env_var;

typedef struct s_shell_context
{
	/* ---------- Environment ---------- */
	t_list *env; // environment variables (KEY=VALUE)
	// char *home;  // cached $HOME value
	/* ---------- Memory / Resource Tracking ---------- */
	t_list				*allocated_pointers[3];
	//// categorized allocation tracking (lifetime-based cleanup)
	t_list				*temporary_files;
	// heredoc files or other temp resources to unlink on exit
	/* ---------- Parsing / Execution State ---------- */
	char *parsing_error; // error message produced during parsing
	bool				in_main_process;
	// true: interactive shell process
	// false: forked child (pipeline stage / subshell)
	int					last_status;
	// exit status of the last executed command ($?)
}						t_shell_context;


typedef enum s_prompt_mode
{
	MAIN_PROMPT,
	HEREDOC_PROMPT
}						t_prompt_mode;


#endif
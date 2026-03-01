/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_context.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:42 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/28 21:46:05 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_CONTEXT_H
# define SHELL_CONTEXT_H

# include "list.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "tracking.h"
# include <stdbool.h>

typedef struct s_env_var
{
	char	*name;
	char	*value;
	bool	exported;
}			t_env_var;

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
/* ---------- Environment ---------- */
// environment variables (KEY=VALUE)
/* ---------- Memory / Resource Tracking ---------- */
//// categorized allocation tracking (lifetime-based cleanup)
// heredoc files or other temp resources to unlink on exit
/* ---------- Parsing / Execution State ---------- */
// error message produced during parsing
// true: interactive shell process
// false: forked child (pipeline stage / subshell)
// true: interactive shell process
// false: forked child (pipeline stage / subshell)
// exit status of the last executed command ($?)

typedef struct s_shell_context
{
	t_list	*env;
	t_list	*allocated_pointers[3];
	t_list	*temporary_files;
	char	*parsing_error;
	bool	in_main_process;
	int		last_status;
}			t_shell_context;

typedef enum s_prompt_mode
{
	MAIN_PROMPT,
	HEREDOC_PROMPT
}			t_prompt_mode;

#endif
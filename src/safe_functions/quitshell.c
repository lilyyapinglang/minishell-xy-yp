#include "minishellparse.h"
#include "parse.h"
#include "safefunctions.h"
#include <readline/history.h>

/*
 * Some readline installations (or headers) don't declare rl_clear_history()
 * even though the symbol exists in the library.
 * We declare it here to avoid implicit-declaration errors under -Werror.
 */
#ifndef HAS_RL_CLEAR_HISTORY_PROTO
void	rl_clear_history(void);
#endif

void	error(const char *context, char *description, int exit_status,
		t_shell_context *sh)
{
	ft_write_fd("minishell: ", STDERR_FILENO);
	ft_write_fd(description, STDERR_FILENO);
	ft_write_fd(": ", STDERR_FILENO);
	ft_write_fd(context, STDERR_FILENO);
	ft_write_fd("\n", STDERR_FILENO);
	quit_shell(exit_status, sh);
}

void	quit_shell(int exit_status, t_shell_context *sh)
{
	if (sh)
	{
		if (sh->in_main_process && isatty(STDIN_FILENO))
			ft_write_fd("exit\n", STDERR_FILENO);
		// clear_prompt(sh); did not clear duing testing
		// ft_lstclear(&sh->env,
		//	free_env_var); did not clean the env part during testing
		ft_lstclear(&sh->allocated_pointers[ALLOC_SHELL], free);
	}
	rl_clear_history();
	exit(exit_status);
}

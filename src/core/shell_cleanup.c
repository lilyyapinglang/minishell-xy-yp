#include "../inc/minishell.h"
#include "../inc/safefunctions.h"
#include "env.h"

static void	cleanup_temp_files(t_shell_context *sh_ctx)
{
	t_list	*node;
	char	*path;

	node = sh_ctx->temporary_files;
	while (sh_ctx->in_main_process && node)
	{
		path = (char *)node->content;
		if (unlink(path) == -1)
			print_errno("unlink", path, errno);
		node = node->next;
	}
	// free (path) + free(node)
	ft_lstclear(&sh_ctx->temporary_files, free);
	sh_ctx->temporary_files = NULL;
}

/*
clear heredoc
cycle allocations
parsing_error
*/

void	shell_clear_iteration(t_shell_context *sh_ctx)
{
	if (!sh_ctx)
		return ;
	cleanup_temp_files(sh_ctx);
	ft_lstclear(&sh_ctx->allocated_pointers[ALLOC_PROMPT], free);
	sh_ctx->parsing_error = NULL;
}

void	shell_destroy(t_shell_context *sh_ctx)
{
	if (!sh_ctx)
		return ;
	shell_clear_iteration(sh_ctx);
	// clear all t_list shell_context->env
	/* env is manually owned */
	ft_lstclear(&sh_ctx->env, free_env_var);
	// clear t_list shell_context->allocated_pointers[TRACK_SHELL]
	/* shell-lifetime tracked allocations */
	ft_lstclear(&sh_ctx->allocated_pointers[ALLOC_SHELL], free);
	// rl_clear_history();
}

void	shell_exit(t_shell_context *sh_ctx, int status)
{
	if (sh_ctx && sh_ctx->in_main_process && isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	shell_destroy(sh_ctx);
	exit(status);
}

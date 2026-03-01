/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 23:20:28 by ylang             #+#    #+#             */
/*   Updated: 2026/02/19 23:22:14 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishellparse.h"
#include "shell_context.h"
#include "parse.h"
#include "safefunctions.h"
#include "shell_context.h"
#include "stdlib.h"
#include "unistd.h"
#include "utils.h"

// clear_prompt(sh); did not clear duing testing
// ft_lstclear(&sh->env,
//	free_env_var); did not clean the env part during testing
// rl_clear_history();
void	quit_shell(int exit_status, t_shell_context *sh)
{
	if (sh)
	{
		if (sh->in_main_process && isatty(STDIN_FILENO))
			ft_write_fd("exit\n", STDERR_FILENO);
		ft_lstclear(&sh->allocated_pointers[ALLOC_SHELL], free);
	}
	exit(exit_status);
}

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

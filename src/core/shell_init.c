/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:43:47 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 19:46:16 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_context.h"

/*
increment shell level at each initiation
*/
void	increment_shlvl(t_shell_context *sh_ctx)
{
	int			level;
	t_list		*node;
	t_env_var	*env_var;
	char		*temp;

	level = 0;
	node = env_node_find(sh_ctx->env, "SHLVL");
	if (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->value)
			level = ft_atoi(env_var->value);
	}
	temp = s_alloc(ft_itoa(level + 1), ALLOC_UNTRACKED, sh_ctx);
	if (!temp)
		return ;
	env_set_value(sh_ctx, "SHLVL", temp, true);
	free(temp);
}

int	readline_hack(void)
{
	return (0);
}

void	init_shell(t_shell_context *sh_ctx, char **envp)
{
	errno = 0;
	sh_ctx->in_main_process = true;
	sh_ctx->last_status = EXIT_SUCCESS;
	sh_ctx->allocated_pointers[ALLOC_SHELL] = NULL;
	sh_ctx->allocated_pointers[ALLOC_PROMPT] = NULL;
	rl_event_hook = &readline_hack;
	sh_ctx->temporary_files = NULL;
	sh_ctx->env = init_env(envp, sh_ctx);
	increment_shlvl(sh_ctx);
	sh_ctx->parsing_error = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:34:47 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 18:39:10 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

void	free_env_var(void *content)
{
	t_env_var	*var;

	var = (t_env_var *)content;
	free(var->name);
	free(var->value);
	free(var);
}

t_env_var	*env_var_from_node(t_list *node)
{
	return ((t_env_var *)node->content);
}

int	env_mark_exported(t_shell_context *sh_ctx, const char *name)
{
	t_list		*node;
	t_env_var	*env_var;

	node = env_node_find(sh_ctx->env, name);
	if (!node)
	{
		add_new_env_var(&sh_ctx->env, name, NULL, true, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	env_var->exported = true;
	return (0);
}

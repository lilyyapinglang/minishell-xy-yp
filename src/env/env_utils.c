/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:34:47 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 20:41:27 by lilypad          ###   ########.fr       */
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

int	env_mark_exported(t_shell_context *ctx, const char *name)
{
	t_list		*node;
	t_env_var	*env_var;

	node = env_node_find(ctx->env, name);
	if (!node)
	{
		add_new_env_var(&ctx->env, name, NULL, true, ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	env_var->exported = true;
	return (0);
}

// envp 导出：只导出 exported==true && value!=NULL
// TODO: garde with malloc_s
char	**build_envp_from_env_list(t_shell_context *sh_ctx)
{
	t_list		*env;
	int			count_strs;
	char		**envp;
	int			i;
	char		*tmp;
	t_env_var	*env_var;

	count_strs = 0;
	i = 0;
	env = sh_ctx->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var->exported && env_var->value)
			count_strs++;
		env = env->next;
	}
	envp = malloc(sizeof(char *) * (count_strs + 1));
	env = sh_ctx->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var->exported && env_var->value)
		{
			tmp = ft_strjoin(env_var->name, "=");
			envp[i++] = ft_strjoin(tmp, env_var->value);
			free(tmp);
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

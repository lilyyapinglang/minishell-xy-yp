/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_apis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:28:37 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/25 22:10:39 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

void	set_export(bool exported, t_env_var *env_var)
{
	if (exported)
		env_var->exported = true;
}

// set/rewrite value
int	env_set_value(t_shell_context *sh_ctx, const char *name, const char *value,
		bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*old;
	char		*new_value;

	if (!sh_ctx || !name || name[0] == '\0')
		return (1);
	node = env_node_find(sh_ctx->env, name);
	if (!node)
	{
		add_new_env_var(&sh_ctx->env, name, value, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	set_export(exported, env_var);
	if (value)
		new_value = strdup_s(value, ALLOC_UNTRACKED, sh_ctx);
	else
		new_value = NULL;
	if (value && !new_value)
		return (1);
	old = env_var->value;
	env_var->value = new_value;
	free(old);
	return (0);
}

// find a env_node
t_list	*env_node_find(t_list *env_list, const char *name)
{
	t_env_var	*env_var;
	t_list		*cur;

	if (!name || name[0] == '\0')
		return (NULL);
	cur = env_list;
	while (cur)
	{
		env_var = env_var_from_node(cur);
		if (env_var && env_var->name && ft_strcmp(env_var->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

// get env value given name
char	*env_get_value(t_list *env, const char *name)
{
	t_env_var	*env_var;
	t_list		*node;

	node = env_node_find(env, name);
	if (!node)
		return (NULL);
	env_var = env_var_from_node(node);
	return (env_var->value);
}

// detele a node from env
// deal differently with head node, last node and middle node

void	del_node_from_env(t_list *env, t_shell_context *sh_ctx)
{
	if (env->prev == NULL)
	{
		if (env->next)
		{
			sh_ctx->env = env->next;
			env->next->prev = NULL;
		}
		else
			sh_ctx->env = NULL;
	}
	else if (env->next == NULL)
		env->prev->next = NULL;
	else
	{
		env->prev->next = env->next;
		env->next->prev = env->prev;
	}
	free_env_var(env->content);
	free(env);
}

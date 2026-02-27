/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:43:52 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/27 17:50:30 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

int	count_exported_env_vars(t_list *node)
{
	t_env_var	*env_var;
	int			count;

	count = 0;
	while (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->exported && env_var->name)
			count++;
		node = node->next;
	}
	return (count);
}

char	*build_env_entry(t_env_var *env_var)
{
	size_t	name_len;
	size_t	val_len;
	char	*val;
	char	*entry;

	if (!env_var || !env_var->name)
		return (NULL);
	name_len = ft_strlen(env_var->name);
	if (env_var->value)
		val = env_var->value;
	else
		val = "";
	val_len = ft_strlen(val);
	entry = malloc(name_len + 1 + val_len + 1);
	if (!entry)
		return (NULL);
	ft_memcpy(entry, env_var->name, name_len);
	entry[name_len] = '=';
	ft_memcpy(entry + name_len + 1, val, val_len);
	entry[name_len + 1 + val_len] = '\0';
	return (entry);
}

char	**free_partial_envp(char **envp, int i)
{
	while (i > 0)
		free(envp[--i]);
	free(envp);
	return (NULL);
}

/* 1) count exported */
/* 2) build envp */
/* rollback */
char	**env_list_to_envp(t_list *env_list)
{
	int			i;
	t_list		*node;
	t_env_var	*env_var;
	char		**envp;

	i = 0;
	node = env_list;
	envp = malloc(sizeof(char *) * (count_exported_env_vars(node) + 1));
	if (!envp)
		return (NULL);
	node = env_list;
	while (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->exported && env_var->name)
		{
			envp[i] = build_env_entry(env_var);
			if (!envp[i])
				return (free_partial_envp(envp, i));
			i++;
		}
		node = node->next;
	}
	envp[i] = NULL;
	return (envp);
}

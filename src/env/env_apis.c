/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_apis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:28:37 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 20:29:34 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

void	add_new_env_var(t_list **env_list, const char *name, const char *value,
		bool exported, t_shell_context *sh_ctx)
{
	t_env_var	*var;

	if (!env_list || !name || name[0] == '\0')
		return ;
	var = calloc_s(1, sizeof(*var), ALLOC_UNTRACKED, sh_ctx);
	var->name = strdup_s(name, ALLOC_UNTRACKED, sh_ctx);
	var->value = value ? strdup_s(value, ALLOC_UNTRACKED, sh_ctx) : strdup_s("",
			ALLOC_UNTRACKED, sh_ctx);
	var->exported = exported;
	lst_add_back_s(var, env_list, ALLOC_UNTRACKED, sh_ctx);
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
void	del_node_from_env(t_list *env, t_shell_context *sh_ctx)
{
	// head node
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
	// last node
	else if (env->next == NULL)
		env->prev->next = NULL;
	else // middle node
	{
		env->prev->next = env->next;
		env->next->prev = env->prev;
	}
	free(env->content);
	free(env);
}

t_list	*sort_by_lexicographical(t_list *head)
{
	int			swapped;
	t_list		*curr;
	t_list		*last;
	t_env_var	*env_var_curr;
	t_env_var	*env_var_next;
	void		*content_tmp;

	// sorting a double linked list
	if (head == NULL)
		return (head);
	last = NULL;
	// keep going until no swaps occur in a pass
	do
	{
		swapped = 0;
		curr = head;
		// traserce through the list and swap adjacent nodes if they are in the wrong order
		while (curr->next != last)
		{
			env_var_curr = env_var_from_node(curr);
			env_var_next = env_var_from_node(curr->next);
			if (ft_strcmp(env_var_curr->name, env_var_next->name) > 0)
			{
				// swap pointer, no need to change data i guess
				// i need to save a pointer
				content_tmp = curr->content;
				curr->content = curr->next->content;
				curr->next->content = content_tmp;
				swapped = 1;
			}
			curr = curr->next;
		}
		last = curr;
	} while (swapped);
	return (head);
}
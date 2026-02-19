/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:34:47 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/19 23:07:38 by ylang            ###   ########.fr       */
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
		add_new_env_var(&sh_ctx->env, name, NULL, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	env_var->exported = true;
	return (0);
}

void	check_swap_node(t_list *curr, int *swapped)
{
	t_env_var	*env_var_curr;
	t_env_var	*env_var_next;
	void		*content_tmp;

	env_var_curr = env_var_from_node(curr);
	env_var_next = env_var_from_node(curr->next);
	if (ft_strcmp(env_var_curr->name, env_var_next->name) > 0)
	{
		content_tmp = curr->content;
		curr->content = curr->next->content;
		curr->next->content = content_tmp;
		*swapped = 1;
	}
}
// sorting a double linked list
// keep going until no swaps occur in a pass
// traserce through the list and
// swap adjacent nodes if they are in the wrong order
// swap pointer, no need to change data i guess

t_list	*sort_by_lexicographical(t_list *head)
{
	t_list	*curr;
	t_list	*last;
	int		swapped;

	if (head == NULL)
		return (head);
	last = NULL;
	swapped = 1;
	while (swapped == 1)
	{
		swapped = 0;
		curr = head;
		while (curr->next != last)
		{
			check_swap_node(curr, &swapped);
			curr = curr->next;
		}
		last = curr;
	}
	return (head);
}

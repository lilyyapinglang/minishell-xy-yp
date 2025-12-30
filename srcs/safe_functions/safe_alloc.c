/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:46:51 by xuewang           #+#    #+#             */
/*   Updated: 2025/12/30 17:14:08 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "safefunctions.h"
#include "parse.h"
#include "minishellparse.h"

void	*s_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh)
{
	if (!pointer)
		error("Memory allocation", strerror(errno), EXIT_FAILURE, sh);
	track_alloc(pointer, scope, sh);
	return (pointer);
}

void	*calloc_s(size_t count, size_t size, t_tracking_scope scope,
		t_shell_context *sh)
{
	return (s_alloc(ft_calloc(count, size), scope, sh));
}

void	track_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh)
{
	t_list	*new_node;

	if (scope == ONLY_CHECK)
		return ;
	new_node = ft_lstnew(pointer);
	check_node_alloc(new_node, pointer, sh);
	ft_lstadd_front(&sh->allocated_pointers[scope], new_node);
}
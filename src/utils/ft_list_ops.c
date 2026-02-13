/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:16:37 by ylang             #+#    #+#             */
/*   Updated: 2026/02/13 21:16:38 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// To be done by yaping lang

// create new node
// t_list	*ft_lstnew(void *content)
// {
// 	t_list	*node;

// 	node = malloc(sizeof(t_list));
// 	if (!node)
// 		return (NULL);
// 	node->content = content;
// 	node->next = NULL;
// 	return (node);
// }
// add node to the front
// TODO
// void	ft_lstadd_front(t_list **lst, t_list *new);
// {
// 	(void)list;
// 	(void)new;
// }

// get list size
// TODO
// int	ft_lstsize(t_list *list)
// {
// 	return (1);
// }
// add node to the back
// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
// 	t_list	*cur;

// 	if (!lst || !new)
// 		return ;
// 	if (*lst == NULL)
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	cur = *lst;
// 	while (cur->next)
// 		cur = cur->next;
// 	cur->next = new;
// }

void	del(void)
{
}
// delete one node
// TODO
// void	ft_lstdelone(t_list *lst, void (*del)(void *));
// {
// 	(void)lst;
// }
// delete entire list
// TODO
// void	ft_lstclear(t_list **lst, void (*del)(void *));
// {
// 	(void)lst;
// }

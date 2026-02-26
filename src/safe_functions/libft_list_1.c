/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_list_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 14:37:54 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 22:36:40 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "safefunctions.h"

//#include "minishellparse.h"
t_list	*ft_lstlast(t_list *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*node;
	void	*mapped_content;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		mapped_content = f(lst->content);
		node = ft_lstnew(mapped_content);
		if (!node)
		{
			del(mapped_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, node);
		lst = lst->next;
	}
	return (new_lst);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*head;

	head = malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->content = content;
	head->next = NULL;
	head->prev = NULL;
	return (head);
}

int	ft_lstsize(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

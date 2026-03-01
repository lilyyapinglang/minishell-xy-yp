/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_list_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 22:34:41 by ylang             #+#    #+#             */
/*   Updated: 2026/02/25 22:36:47 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "safefunctions.h"

void	ft_lstadd_back(t_list **lst, t_list *n)
{
	t_list	*tmp;

	if (!*lst)
		*lst = n;
	else
	{
		tmp = ft_lstlast(*lst);
		tmp->next = n;
		n->prev = tmp;
	}
}

void	ft_lstadd_front(t_list **lst, t_list *n)
{
	n->next = *lst;
	if (*lst != NULL)
		(*lst)->prev = n;
	*lst = n;
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*buffer;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		buffer = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = buffer;
	}
	*lst = NULL;
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

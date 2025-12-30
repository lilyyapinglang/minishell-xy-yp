#include "../inc/minishell.h"

// create new node
t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
// add node to the front
void	ft_lstadd_front(t_list **lst, t_list *new);
{
	t_list **head;
	head = lst;
}

// get list size
int	ft_lstsize(t_list *list)
{
}
// add node to the back
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cur;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

// delete one node
void	ft_lstdelone(t_list *lst, void (*del)(void *));
{
}
// delete entire list
void	ft_lstclear(t_list **lst, void (*del)(void *));
{
}

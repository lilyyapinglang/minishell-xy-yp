

ft_lst_remove(&shell_context->env, env);


ft_lst_add(&env_list, env_var);

t_list *ft_lstnew(void *content)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->content = content;
    node->next = NULL;
    return node;
}
void ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list *cur;

    if (!lst || !new)
        return;
    if (*lst == NULL)
    {
        *lst = new;
        return;
    }
    cur = *lst;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

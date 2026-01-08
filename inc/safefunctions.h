#ifndef SAFEFUNCTIONS_H
# define SAFEFUNCTIONS_H

# include "list.h"
# include "minishell.h"
# include "parse.h"
# include "tracking.h"
# include <stdbool.h>

typedef struct s_shell_context	t_shell_context;
/*libft functions*/

t_list							*ft_lstnew(void *content);
int								ft_lstsize(t_list *lst);
t_list							*ft_lstlast(t_list *lst);
void							ft_lstadd_front(t_list **lst, t_list *n);
void							ft_lstadd_back(t_list **lst, t_list *n);
void							ft_lstiter(t_list *lst, void (*f)(void *));
void							ft_lstdelone(t_list *lst, void (*del)(void *));
void							ft_lstclear(t_list **lst, void (*del)(void *));

t_list							*ft_lstmap(t_list *lst, void *(*f)(void *),
									void (*del)(void *));

/*safe alloc*/

void							*s_alloc(void *pointer, t_tracking_scope scope,
									t_shell_context *sh);
void							*calloc_s(size_t count, size_t size,
									t_tracking_scope scope,
									t_shell_context *sh);
void							track_alloc(void *pointer,
									t_tracking_scope scope,
									t_shell_context *sh);

/*safe list*/
void							lst_add_front_s(void *content, t_list **lst,
									t_tracking_scope scope,
									t_shell_context *sh);
void							lst_add_back_s(void *content, t_list **lst,
									t_tracking_scope scope,
									t_shell_context *sh);
void							check_node_alloc(t_list *new_node,
									void *pointer, t_shell_context *sh);
void							remove_list_node(t_list **node, t_list **head,
									void (*free_function)(void *),
									bool free_node);

/*secure libft*/
char							*strdup_s(char const *str1,
									t_tracking_scope scope,
									t_shell_context *sh_ctx);
#endif
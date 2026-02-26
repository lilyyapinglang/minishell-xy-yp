/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safefunctions.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:12:36 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 20:12:37 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAFEFUNCTIONS_H
# define SAFEFUNCTIONS_H

# include "list.h"
# include "shell_context.h"
# include "tracking.h"
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/types.h>

/*libft functions*/

t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_front(t_list **lst, t_list *n);
void	ft_lstadd_back(t_list **lst, t_list *n);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/*safe alloc*/

void	*s_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);
void	*calloc_s(size_t count, size_t size, t_tracking_scope scope,
			t_shell_context *sh);
void	track_alloc(void *pointer, t_tracking_scope scope, t_shell_context *sh);

/*safe list*/

void	lst_add_front_s(void *content, t_list **lst, t_tracking_scope scope,
			t_shell_context *sh);
void	lst_add_back_s(void *content, t_list **lst, t_tracking_scope scope,
			t_shell_context *sh);
void	check_node_alloc(t_list *new_node, void *pointer, t_shell_context *sh);
void	remove_list_node(t_list **node, t_list **head,
			void (*free_function)(void *), bool free_node);

/*safe libft*/
char	*strjoin_s(char const *s1, char const *s2, t_tracking_scope scope,
			t_shell_context *sh);
char	**split_s(char const *s, char c, t_tracking_scope scope,
			t_shell_context *sh);
char	*strdup_s(char const *s1, t_tracking_scope scope, t_shell_context *sh);
ssize_t	write_s(const char *s, int fd, t_shell_context *sh);

void	add_arg_to_array(char ***array, char *new_arg, t_shell_context *sh);

/*safe file*/
int		open_s(const char *pathname, int flags, mode_t mode,
			t_shell_context *sh_ctx);
int		close_s(int fd, t_shell_context *sh_ctx);
int		dup2_s(int oldfd, int newfd, t_shell_context *sh_ctx);
// int		stat_s(const char *restrict path, struct stat *restrict buf,
// 			t_shell_context *sh_ctx);
#endif
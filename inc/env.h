/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:07:43 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/28 21:47:59 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "list.h"
# include "shell_context.h"

# define DEFAULT_PATH \
	"/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

t_list		*init_env(char **envp, t_shell_context *sh_ctx);
void		print_env_or_export(bool export_format, t_shell_context *sh_ctx);
// void		add_new_env_var(t_list **env_list, const char *name,
// 				const char *value, bool exported, t_shell_context *sh_ctx);
void		add_new_env_var(t_list **env_list, const char *name,
				const char *value, t_shell_context *sh_ctx);
// env apis

void		del_node_from_env(t_list *env, t_shell_context *sh_ctx);
t_list		*sort_by_lexicographical(t_list *head);
void		set_export(bool exported, t_env_var *env_var);

t_list		*env_node_find(t_list *env, const char *name);
char		*env_get_value(t_list *env, const char *name);
int			env_set_value(t_shell_context *sh_ctx, const char *name,
				const char *value, bool exported);
int			env_append_value(t_shell_context *sh_ctx, const char *name,
				const char *append_str, bool exported);
int			env_unset(t_shell_context *sh_ctx, const char *name);
int			env_mark_exported(t_shell_context *sh_ctx, const char *name);
t_env_var	*env_var_from_node(t_list *node);
void		free_env_var(void *content);

// build envp from t_list env
char		**env_list_to_envp(t_list *env_list);
#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:44:57 by lilypad           #+#    #+#             */
/*   Updated: 2026/03/01 11:43:16 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*search_path_value(t_list *env)
{
	t_env_var	*env_var;

	while (env)
	{
		env_var = env_var_from_node(env);
		if (!env_var)
		{
			env = env->next;
			continue ;
		}
		if (ft_strcmp(env_var->name, "PATH") == 0)
			return (env_var->value);
		env = env->next;
	}
	return (NULL);
}

static char	*find_executable(char **dirs, char *cmd)
{
	char	*mid_path;
	char	*full_path;

	while (*dirs)
	{
		mid_path = ft_strjoin(*dirs, "/");
		if (!mid_path)
			return (NULL);
		full_path = ft_strjoin(mid_path, cmd);
		free(mid_path);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		dirs++;
	}
	return (NULL);
}

// PATH=/home/ylang/bin:/home/ylang/bin:/
// try ./cmd
// check for execution permission
// free dirs up to now
// free strs too
char	*resolve_cmd_path(char *cmd, t_shell_context *sh_ctx)
{
	char	**dirs;
	char	*path_ptr;
	t_list	*env;
	char	*result;

	path_ptr = NULL;
	env = sh_ctx->env;
	path_ptr = search_path_value(env);
	if (!path_ptr)
		return (ft_strjoin("./", cmd));
	if (*path_ptr == '\0')
		return (NULL);
	dirs = ft_split(path_ptr, ':');
	if (!dirs)
		return (NULL);
	result = find_executable(dirs, cmd);
	free_strs(dirs);
	return (result);
}

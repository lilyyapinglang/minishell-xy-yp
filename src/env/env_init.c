/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:06:19 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/11 20:34:29 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

/*
env：打印 exported==true && value!=NULL
export：打印 exported==true（value 可空）
envp 导出：只导出 exported==true && value!=NU
export 的作用是：让某个变量进入将来子进程的 envp（即“导出”）。
exported 的意义：
这个变量是否应该被放进 env_to_char_array() 生成的 envp（传给 execve）。
*/

// set/rewrite value
int	env_set_value(t_shell_context *sh_ctx, const char *name, const char *value,
		bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*old;
	char		*new_value;

	if (!sh_ctx || !name || name[0] == '\0')
		return (1);
	node = env_node_find(sh_ctx->env, name);
	if (!node)
	{
		add_new_env_var(&sh_ctx->env, name, value, exported, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	if (exported)
		env_var->exported = true;
	new_value = value ? strdup_s(value, ALLOC_UNTRACKED, sh_ctx) : NULL;
	if (value && !new_value)
		return (1);
	old = env_var->value;
	env_var->value = new_value;
	free(old);
	return (0);
}

// append VAR +=
int	env_append_value(t_shell_context *sh_ctx, const char *name,
		const char *append_str, bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*base;
	char		*new_value;

	node = env_node_find(sh_ctx->env, name);
	if (!node)
	{
		add_new_env_var(&sh_ctx->env, name, append_str, exported, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	if (exported)
		env_var->exported = true;
	base = env_var->value ? env_var->value : strdup_s("", ALLOC_UNTRACKED,sh_ctx);
	new_value = ft_strjoin(base, append_str ? append_str : "");
	if (env_var->value == NULL)
		free(base);
	free(env_var->value);
	env_var->value = new_value;
	return (0);
}

// unset env
int	env_unset(t_shell_context *sh_ctx, const char *name)
{
	t_list		*env;
	t_env_var	*env_var;

	env = sh_ctx->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (ft_strcmp(env_var->name, name) == 0)
		{
			del_node_from_env(env, sh_ctx);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

t_list	*init_env(char **envp, t_shell_context *sh_ctx)
{
	t_list	*env_list;
	char	*equal_sign_loc;
	char	*name_tmp;

	env_list = NULL;
	if (!sh_ctx)
		return (NULL);
	while (envp && *envp)
	{
		equal_sign_loc = ft_strchr(*envp, '=');
		if (!equal_sign_loc) // no =
		{
			// No '=' -> name only, value NULL
			// exported=true because it came from envp
			add_new_env_var(&env_list, *envp, NULL, true, sh_ctx);
		}
		else 
		{
			// TODO: maybe need ft_substr_s
			name_tmp = ft_substr(*envp, 0, (size_t)(equal_sign_loc - *envp));
			if (name_tmp)
			{
				add_new_env_var(&env_list, name_tmp, equal_sign_loc + 1, true,
					sh_ctx);
				free(name_tmp);
			}
		}
		envp++;
	}
	sh_ctx->env = env_list;
	if (!env_node_find(env_list, "PATH"))
		add_new_env_var(&env_list, "PATH", DEFAULT_PATH, true, sh_ctx);
	return (env_list);
}

// called by  builtin-env and built-in export
// when print env, "" will be printed
// when print export,  in format  "declare -x", sort by env->name,
// pay attention to	value = ""
void	print_env(bool export_format, t_shell_context *sh_ctx)
{
	t_env_var *env_var;
	t_list *env = sh_ctx->env;
	t_list *sorted_env;

	if (!export_format)
	{
		while (env)
		{
			env_var = env_var_from_node(env);
			printf("%s=%s\n", env_var->name, env_var->value);
			env = env->next;
		}
	}
	else
	{
		sorted_env = sort_by_lexicographical(env);
		while (sorted_env)
		{
			env_var = env_var_from_node(sorted_env);
			if (!env_var->exported || ft_strcmp(env_var->name, "_") == 0)
			{
				sorted_env = sorted_env->next;
				continue ;
			}
			printf("declare -x %s=\"%s\"\n", env_var->name, env_var->value);
			sorted_env = sorted_env->next;
		}
	}
}
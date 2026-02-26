/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:49:04 by ylang             #+#    #+#             */
/*   Updated: 2026/02/19 23:28:13 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "env.h"
#include "safefunctions.h"
#include "stdio.h" //testonly
#include "utils.h"

void	print_env(t_list *env)
{
	t_env_var	*env_var;

	while (env)
	{
		env_var = env_var_from_node(env);
		printf("%s=%s\n", env_var->name, env_var->value);
		env = env->next;
	}
}

void	print_export(t_list *env)
{
	t_list		*sorted_env;
	t_env_var	*env_var;

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

// called by  builtin-env and built-in export
// when print env, "" will be printed
// when print export,  in format  "declare -x", sort by env->name,
// pay attention to	value = ""
void	print_env_or_export(bool export_format, t_shell_context *sh_ctx)
{
	t_list	*env;

	env = sh_ctx->env;
	if (!export_format)
		print_env(env);
	else
		print_export(env);
}

char	*appended_value(t_env_var *env_var, char *base, const char *append_str,
		t_shell_context *sh_ctx)
{
	char	*appended_value;

	if (env_var->value)
		base = env_var->value;
	else
		base = strdup_s("", ALLOC_UNTRACKED, sh_ctx);
	if (!append_str)
		append_str = "";
	appended_value = strjoin_s(base, append_str, ALLOC_UNTRACKED, sh_ctx);
	return (appended_value);
}

// append VAR +=
int	env_append_value(t_shell_context *sh_ctx, const char *name,
		const char *append_str, bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*base;
	char		*new_value;

	base = NULL;
	node = env_node_find(sh_ctx->env, name);
	if (!node)
	{
		add_new_env_var(&sh_ctx->env, name, append_str, sh_ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	set_export(exported, env_var);
	new_value = appended_value(env_var, base, append_str, sh_ctx);
	if (env_var->value == NULL)
		free(base);
	free(env_var->value);
	env_var->value = new_value;
	return (0);
}

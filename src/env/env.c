#include "../inc/minishell.h"

/*

env：打印 exported==true && value!=NULL

export：打印 exported==true（value 可空）

envp 导出：只导出 exported==true && value!=NULL

export 的作用是：让某个变量进入将来子进程的 envp（即“导出”）。
exported 的意义：
这个变量是否应该被放进 env_to_char_array() 生成的 envp（传给 execve）。
*/

static t_env_var	*env_var_from_node(t_list *node)
{
	return ((t_env_var *)node->content);
}
int	add_new_env_var(t_list **env, const char *name, const char *value,
		bool exported)
{
	t_env_var	*env_var;
	t_list		*node;

	if (!env || !name || name[0] == '\0')
		return (1);
	env_var = malloc(sizeof(*env_var));
	if (!env_var)
		return (1);
	env_var->name = ft_strdup(name);
	if (!env_var->name)
		return (free(env_var), 1);
	env_var->value = value ? ft_strdup(value) : NULL;
	if (value && !env_var->value)
		return (free(env_var->name), free(env_var), 1);
	env_var->exported = exported;
	node = ft_lstnew(env_var);
	if (!node)
		return (free(env_var->value), free(env_var->name), free(env_var), 1);
	ft_lstadd_back(env, node);
	return (0);
}

int	env_mark_exported(t_shell_context *ctx, const char *name)
{
	t_list		*node;
	t_env_var	*env_var;

	node = env_node_find(ctx->env, name);
	if (!node)
		return (add_new_env_var(&ctx->env, name, NULL, true));
	env_var = env_var_from_node(node);
	env_var->exported = true;
	return (0);
}

// return env_node
t_list	*env_node_find(t_list *env, const char *name)
{
	t_env_var	*env_var;
	size_t		n;

	while (env)
	{
		env_var = env_var_from_node(env);
		n = ft_strlen(env_var->name);
		if (env_var && env_var->name && ft_strncmp(env_var->name, name, n) == 0
			&& env_var->name[n] == '\0')
			return (env);
		env = env->next;
	}
	return (NULL);
}
// get value
char	*env_get_value(t_list *env, const char *name)
{
	t_env_var	*env_var;
	t_list		*node;

	node = env_node_find(env, name);
	if (!node)
		return (NULL);
	env_var = env_var_from_node(node);
	return (env_var->value);
}
// set/rewrite value
int	env_set_value(t_shell_context *shell_context, const char *name,
		const char *value, bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*old;

	node = env_node_find(shell_context->env, name);
	if (!node)
	{
		add_new_env_var(&shell_context->env, name, value, exported);
		return (0);
	}
	env_var = env_var_from_node(node);
	// udpate exported is requested
	if (exported)
		env_var->exported = true;
	// repalce value
	old = env_var->value;
	env_var->value = value ? ft_strdup(value) : NULL;
	free(old);
	return (0);
}

// append VAR +=
int	env_append_value(t_shell_context *shell_context, const char *name,
		const char *append_str, bool exported)
{
	t_env_var	*env_var;
	t_list		*node;
	char		*base;
	char		*new_value;

	node = env_node_find(shell_context->env, name);
	if (!node)
	{
		add_new_env_var(&shell_context->env, name, append_str, exported);
		return (0);
	}
	env_var = env_var_from_node(node);
	// udpate exported is requested
	if (exported)
		env_var->exported = true;
	base = env_var->value ? env_var->value : ft_strdup("");
	new_value = ft_strjoin(base, append_str ? append_str : "");
	if (env_var->value == NULL)
		free(base);
	free(env_var->value);
	env_var->value = new_value;
	return (0);
}

// delete node
int	env_unset(t_shell_context *shell_context, const char *name)
{
	t_list		*env;
	t_env_var	*env_var;

	env = shell_context->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (ft_strncmp(env_var->name, name, ft_strlen(name)) == 0)
		{
			// detele this node from lsit
			// TODO
			ft_lstdelone(&shell_context->env, free);
			return (0);
		}
		env = env->next;
	}
	return (1);
}
// envp 导出：只导出 exported==true && value!=NULL
char	**build_envp_from_env_list(t_shell_context *shell_context)
{
	t_list		*env;
	int			count_strs;
	char		**envp;
	int			i;
	char		*tmp;
	t_env_var	*env_var;

	count_strs = 0;
	i = 0;
	env = shell_context->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var->exported && env_var->value)
			count_strs++;
		env = env->next;
	}
	envp = malloc(sizeof(char *) * (count_strs + 1)); // garde with malloc_s
	env = shell_context->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var->exported && env_var->value)
		{
			tmp = ft_strjoin(env_var->name, "=");
			envp[i++] = ft_strjoin(tmp, env_var->value);
			free(tmp);
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

t_list	*init_env(char **envp, t_shell_context *shell_context)
{
	t_list	*env_list;
	char	*equal_sign;
	char	*name_tmp;

	env_list = NULL;
	if (!shell_context)
		return (NULL);
	while (envp && *envp)
	{
		equal_sign = ft_strchr(*envp, '=');
		if (!equal_sign)
		{
			// No '=' -> name only, value NULL
			// exported=true because it came from envp
			add_new_env_var(&env_list, *envp, NULL, true, shell_context);
		}
		else
		{
			// name is left side
			name_tmp = ft_substr(*envp, 0, (size_t)(equal_sign - *envp));
			if (name_tmp)
			{
				// value is right side (eq+1)
				add_new_env_var(&env_list, name_tmp, equal_sign + 1, true,
					shell_context);
				free(name_tmp);
			}
			// else
			// {
			// 	// if you have malloc_s/calloc_s that exits, you won't need this
			// 	// otherwise decide how you want to handle alloc failure
			// }
		}
		envp++;
	}
	shell_context->env = env_list;
	// Recommendation: do NOT cache HOME here (avoids stale cache bugs)
	// If you insist on caching:
	// shell_context->home = env_get_value(env_list, "HOME");
	if (!env_node_find(env_list, "PATH"))
		add_new_env_var(&env_list, "PATH", DEFAULT_PATH, true, shell_context);
	return (env_list);
}

// called by on builtin-env or built-in export ?
void	print_env(bool export_format, t_shell_context *shell_context)
{
	t_env_var *env_var;
	t_list *env = shell_context->env;

	while (env)
	{
		// env , exporte_format = false, only print exporetd && value!=NULL
		env_var = env_var_from_node(env);
		if (!export_format)
		{
			// env builtin
			if (env_var->exported == false || env_var->value == NULL)
			{
				env = env->next;
				continue ;
			}
			printf("%s=%s\n", env_var->name, env_var->value);
		}
		// export , export_formate = true, only print exported,
		//	value could be NULL,
		// in format "declare -x"
		else
		{
			if (!env_var->exported)
			{
				env = env->next;
				continue ;
			}
			if (env_var->value == NULL)
				printf("declare -x %s\n", env_var->name);
			else
				printf("declare -x %s=\"%s\"\n", env_var->name, env_var->value);
		}
		env = env->next;
	}
}
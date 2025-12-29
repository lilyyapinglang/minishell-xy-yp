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
void				add_new_env_var(t_list *env, const char *name,
						const char *value, bool exported);
{
}
// return env_node
t_list	*env_node_find(t_list *env, const char *name)
{
	t_env_var	*env_var;

	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var && env_var->name && ft_strncmp(env_var->name, name),
			ft_strlen(name) == 0)
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
	env_var = env_var_from_node(env);
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
	char		*old;
	char		*base;
	char		*new_value;

	node = env_node_find(shell_context->env, name);
	if (!node)
	{
		add_new_env_var(&shell_context->env, name, value, exported);
		return (0);
	}
	env_var = env_var_from_node(node);
	env_var = env_var_from_node(node);
	// udpate exported is requested
	if (exported)
		env_var->exported = true;
	base = env_var->value ? env_var->value : ft_strdup("");
	new_value = ft_strjoin(base, append_str ? append_str : "");
	if (env_var->value = NULL)
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

	env_var = ((t_env_var *)env->content);
	env = shell_context->env;
	while (env)
	{
		if (ft_strncmp(env_var->name, name), ft_strlen(name) == 0)
		{
			// detele this node from lsit
			ft_lst_remove(&shell_context->env, env);
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

	env_var = ((t_env_var *)env->content);
	count_strs = 0;
	i = 0;
	env = shell_context->env;
	while (env)
	{
		if (env_var->exported == true && env_var->value)
			count_strs++;
		env = env->next;
	}
	envp = malloc(sizeof(char *) * (count_strs + 1)); // garde with malloc_s
	env = shell_context->env;
	while (env)
	{
		if (env_vart->exported == true && env_var->value)
		{
			tmp = ft_strjoin(env_var->name, "=");
			envp[i] = ft_strjoin(tmp, env_var->value);
			free(tmp);
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	init_env(t_shell_context *shell_context, char **envp)
{
	t_env_var	*env_var;
	char		*name;
	char		*value;
	char		equal_sign;
	char		**splited;
	t_list		*env_list;
	int			eq_loc;

	equal_sign = '=';
	// build envp into a t_list env tree ?
	while (*envp)
	{
		eq_loc = ft_strchr(*envp, equal_sign);
		if (!eq_loc) // = does not exist
		{
			env_var->name = ft_strdup(*envp);
			env_var->value = NULL;
		}
		else
		{
			env_var->name = ft_substr(0, eq_loc - 1);
			env_var->value = eq_loc + 1;
		}
		// because initially read from envp so all exported = true,
		// because they orignaly are envorienmtent
		env_var->exported = true;
		ft_lst_add(&env_list, env_var);
		envp++;
	}
	shell_context->env = env_list;
	shell_context->home = env_get_value(env_list, "PATH");
}
// called by on builtin-env or built-in export ?
void	print_env(bool export_format, t_shell_context *shell_context)
{
	t_env_var *env_var;
	t_list *env = shell_context.env;

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
			printf("%s=%s", env_var->name, env_var->value);
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
			if (env_var.value == NULL)
				printf("declare -x %s\n", env_var->name);
			else
				printf("decalre -x %s= \"%s\"\n", env_var->name,
					env_var->value);
		}
		env = env->next;
	}
}
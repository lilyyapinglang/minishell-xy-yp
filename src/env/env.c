#include "../inc/minishell.h"

/*

env：打印 exported==true && value!=NULL

export：打印 exported==true（value 可空）

envp 导出：只导出 exported==true && value!=NULL

export 的作用是：让某个变量进入将来子进程的 envp（即“导出”）。
exported 的意义：
这个变量是否应该被放进 env_to_char_array() 生成的 envp（传给 execve）。
*/
typedef struct s_env_var
{
	char	*name;
	char	*value;
	bool	*exported;
}			t_env_var;

void		add_new_env_var(t_list *env, const char *name, const char *value,
				bool exported);
{
}
// return env_node
t_list	*env_node_find(t_list *env, const char *name)
{
	while (env)
	{
		if ((t_env_var)env->content->name == name)
			return (env);
		env = env->next;
	}
	return (NULL);
}
// get value
char	*env_get_value(t_list *env, const char *name)
{
	while (env)
	{
		if ((t_env_var)env->content->name == name)
			return ((t_env_var)env->content->value);
		env = env->next;
	}
	return (NULL);
}
// set/rewrite value
int	env_set_value(t_shell_context *shell_context, const char *name,
		const char *value, bool exported)
{
	t_list	*env;

	env = shell_context->env;
	while (env)
	{
		if ((t_env_var)env->content->name == name)
		{
			env->content->value = value;
			return (0);
		}
		env = env->next;
	}
	return (1);
}

// append VAR +=
int	env_append_value(t_shell_context *shell_context, const char *name,
		const char *append_str, bool exported)
{
	t_list	*env;
	char	*tmp;

	env = shell_context->env;
	while (env)
	{
		if ((t_env_var)env->content->name == name)
		{
			tmp = env->content->value;
			env->content->value = ft_strjoin(tmp, append_str);
			free(tmp);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

// delete node
int	env_unset(t_shell_context *shell_context, const char *name)
{
	t_list	*env;

	env = shell_context->env;
	while (env)
	{
		if ((t_env_var)env->content->name == name)
		{
			// detele this node from lsit
			ft_lst_remove(&env, env);
			return (0);
		}
		env = env->next;
	}
	return (1);
}
// envp 导出：只导出 exported==true && value!=NULL
char	**build_envp_from_env(t_shell_context *shell_context)
{
	t_list	*env;
	int		count_strs;
	char	*envp;
	int		i;
	char	*tmp;

	count_strs = 0;
	i = 0;
	env = shell_context->env;
	while (env)
	{
		if ((t_env_var)env->content->exported == true
			&& (t_env_var)env->content->value)
			count_strs++;
		env = env->next;
	}
	envp = malloc(sizeof(char *) * (count_strs + 1)); // garde with malloc_s
	while (env)
	{
		if ((t_env_var)env->content->exported == true
			&& (t_env_var)env->content->value)
		{
			tmp = ft_strjoin((t_env_var)env->content->exported, "=");
			envp[i] = ft_strjoin(tmp, (t_env_var)env->content->value);
			free(tmp);
		}
		env = env->next;
	}
	return (envp);
}

void	init_env(t_shell_context *shell_context, char **envp)
{
	t_env_var	*env_var;
	char		*name;
	char		*value;
	char		*equal_sign;
	char		**splited;
	t_list		*env_list;

	// build envp into a t_list env tree ?
	while (*envp)
	{
		if (!ft_strchr(equal_sign))
			name = *envp;
		if (ft_strchr(equal_sign))
		{
			splited = ft_split(*envp, equal_sign);
			env_var->name = splited[0];
			env_var->value = splited[1];
			// because initially read from envp so all exported = true,
			// because they orignaly are envorienmtent
			env_var->exported = true;
			ft_lst_add(&env_list, env_var);
		}
		envp++;
	}
	shell_context->env = env_list;
	shell_context->home = env_get_value(env_list, "PATH");
}
// depends on env or export ?
void	print_env(bool export_format, t_shell_context *shell_context)
{
	// env , exporte_format = false, only print exporetd && value!=NULL

	// export , export_formate = true, only print exported, value could be NULL,
	// in format "declare -x"
}
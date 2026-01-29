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

void	free_env_var(void *content)
{
	t_env_var	*var;

	var = (t_env_var *)content;
	free(var->name);
	free(var->value);
	free(var);
}

t_env_var	*env_var_from_node(t_list *node)
{
	return ((t_env_var *)node->content);
}

void	add_new_env_var(t_list **env_list, const char *name, const char *value,
		bool exported, t_shell_context *sh_ctx)
{
	t_env_var	*var;

	if (!env_list || !name || name[0] == '\0')
		return ;
	var = calloc_s(1, sizeof(*var), ALLOC_UNTRACKED, sh_ctx);
	var->name = strdup_s(name, ALLOC_UNTRACKED, sh_ctx);
	var->value = value ? strdup_s(value, ALLOC_UNTRACKED, sh_ctx) : NULL;
	var->exported = exported;
	// node = ft_listnew_s(var, TRACK_SHELL, sh_ctx);
	lst_add_back_s(var, env_list, ALLOC_UNTRACKED, sh_ctx);
}

int	env_mark_exported(t_shell_context *ctx, const char *name)
{
	t_list		*node;
	t_env_var	*env_var;

	node = env_node_find(ctx->env, name);
	if (!node)
	// return (add_new_env_var(&ctx->env, name, NULL, true, ctx));
	{
		add_new_env_var(&ctx->env, name, NULL, true, ctx);
		return (0);
	}
	env_var = env_var_from_node(node);
	env_var->exported = true;
	return (0);
}

// return env_node
t_list	*env_node_find(t_list *env_list, const char *name)
{
	t_env_var	*env_var;
	t_list		*cur;

	if (!name || name[0] == '\0')
		return (NULL);
	cur = env_list;
	while (cur)
	{
		env_var = env_var_from_node(cur);
		// printf("eng_Var");
		if (env_var && env_var->name && ft_strcmp(env_var->name, name) == 0)
			return (cur);
		cur = cur->next;
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
	// udpate exported is requested
	if (exported)
		env_var->exported = true;
	new_value = value ? ft_strdup(value) : NULL;
	if (value && !new_value)
		return (1);
	// repalce value
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
void	del_node_from_env(t_list *env, t_shell_context *sh_ctx)
{
	// head node
	if (env->prev == NULL)
	{
		if (env->next)
		{
			sh_ctx->env = env->next;
			env->next->prev = NULL;
		}
		else
			sh_ctx->env = NULL;
	}
	// last node
	else if (env->next == NULL)
		env->prev->next = NULL;
	else // middle node
	{
		env->prev->next = env->next;
		env->next->prev = env->prev;
	}
	free(env->content);
	free(env);
}
// delete node
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
			// detele this node from lsit
			// need to handle delete at head, at middle or at the en
			// ft_lstdelone(sh_ctx->env, free);
			del_node_from_env(env, sh_ctx);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

// envp 导出：只导出 exported==true && value!=NULL
char	**build_envp_from_env_list(t_shell_context *sh_ctx)
{
	t_list		*env;
	int			count_strs;
	char		**envp;
	int			i;
	char		*tmp;
	t_env_var	*env_var;

	count_strs = 0;
	i = 0;
	env = sh_ctx->env;
	while (env)
	{
		env_var = env_var_from_node(env);
		if (env_var->exported && env_var->value)
			count_strs++;
		env = env->next;
	}
	envp = malloc(sizeof(char *) * (count_strs + 1)); // garde with malloc_s
	env = sh_ctx->env;
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

t_list	*init_env(char **envp, t_shell_context *sh_ctx)
{
	t_list	*env_list;
	char	*equal_sign;
	char	*name_tmp;

	env_list = NULL;
	if (!sh_ctx)
		return (NULL);
	while (envp && *envp)
	{
		equal_sign = ft_strchr(*envp, '=');
		if (!equal_sign)
		{
			// No '=' -> name only, value NULL
			// exported=true because it came from envp
			add_new_env_var(&env_list, *envp, NULL, true, sh_ctx);
		}
		else
		{
			// name is left side
			// TODO: maybe need ft_substr_s
			name_tmp = ft_substr(*envp, 0, (size_t)(equal_sign - *envp));
			if (name_tmp)
			{
				// value is right side (eq+1)
				add_new_env_var(&env_list, name_tmp, equal_sign + 1, true,
					sh_ctx);
				free(name_tmp);
			}
		}
		envp++;
	}
	sh_ctx->env = env_list;
	// Recommendation: do NOT cache HOME here (avoids stale cache bugs)
	// If you insist on caching:
	// sh_ctx->home = env_get_value(env_list, "HOME");
	if (!env_node_find(env_list, "PATH"))
		add_new_env_var(&env_list, "PATH", DEFAULT_PATH, true, sh_ctx);
	return (env_list);
}

// called by on builtin-env or built-in export ?
void	print_env(bool export_format, t_shell_context *sh_ctx)
{
	t_env_var *env_var;
	t_list *env = sh_ctx->env;

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
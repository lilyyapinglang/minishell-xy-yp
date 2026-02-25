/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 20:06:19 by lilypad           #+#    #+#             */
/*   Updated: 2026/02/23 22:29:46 by ylang            ###   ########.fr       */
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
// void	add_new_env_var(t_list **env_list, const char *name, const char *value,
// bool exported, t_shell_context *sh_ctx)
void	add_new_env_var(t_list **env_list, const char *name, const char *value,
		t_shell_context *sh_ctx)
{
	t_env_var	*var;

	if (!env_list || !name || name[0] == '\0')
		return ;
	var = calloc_s(1, sizeof(*var), ALLOC_UNTRACKED, sh_ctx);
	var->name = strdup_s(name, ALLOC_UNTRACKED, sh_ctx);
	if (value)
		var->value = strdup_s(value, ALLOC_UNTRACKED, sh_ctx);
	else
		var->value = strdup_s("", ALLOC_UNTRACKED, sh_ctx);
	var->exported = true;
	lst_add_back_s(var, env_list, ALLOC_UNTRACKED, sh_ctx);
}

void	add_env_var_with_value(char *envp, char *equal_sign_loc,
		t_list **env_list, t_shell_context *sh_ctx)
{
	char	*name_tmp;

	name_tmp = ft_substr(envp, 0, (size_t)(equal_sign_loc - envp));
	if (name_tmp)
	{
		add_new_env_var(env_list, name_tmp, equal_sign_loc + 1, sh_ctx);
		free(name_tmp);
	}
}

// No '=' -> name only, value NULL
// exported=true because it came from envp
t_list	*init_env(char **envp, t_shell_context *sh_ctx)
{
	t_list	*env_list;
	char	*equal_sign_loc;

	env_list = NULL;
	if (!sh_ctx)
		return (NULL);
	while (envp && *envp)
	{
		equal_sign_loc = ft_strchr(*envp, '=');
		if (!equal_sign_loc)
			add_new_env_var(&env_list, *envp, NULL, sh_ctx);
		else
			add_env_var_with_value(*envp, equal_sign_loc, &env_list, sh_ctx);
		envp++;
	}
	if (!env_node_find(env_list, "PATH"))
		add_new_env_var(&env_list, "PATH", DEFAULT_PATH, sh_ctx);
	sh_ctx->env = env_list;
	return (env_list);
}

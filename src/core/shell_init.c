#include "../inc/minishell.h"


// TODO:
// increment shell level at each initiation
void	increment_shlvl(t_shell_context *sh_ctx)
{
	int			level;
	t_list		*node;
	t_env_var	*env_var;
	char		*temp;

	level = 0;
	node = env_node_find(shell_context->env, "SHLVL");
	if (node)
	{
		env_var = env_var_from_node(node);
		if (env_var && env_var->value)
			level = ft_atoi(env_var->value)
	}
	temp = ft_itoa(level + 1);
	if (!temp)
		return ;
	env_set_value(shell_context, "SHLVL", temp, true, shell_context);
	free(temp);
}
void	init_shell(t_shell_context *sh_ctx, char **envp)
{
	errno = 0;
	shell_context->in_main_process = true;
	shell_context->last_status = EXIT_SUCCESS;
	shell_context->allocated_pointers[ALLOC_SHELL] = NULL;
	shell_context->allocated_pointers[ALLOC_PROMPT] = NULL;
	// allocated_pointers[TRACK_NONE] is unused because ONLY_CHECK allocations are not tracked
	shell_context->temporary_files = NULL;
	shell_context->env = init_env(envp, shell_context);
	increment_shlvl(shell_context);
	shell_context->parsing_error = NULL;
}
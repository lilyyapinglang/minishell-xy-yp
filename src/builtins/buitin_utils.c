#include "../inc/minishell.h"

typedef int			(*t_builtin_fn)(char **argv, t_shell_context *ctx);

typedef enum e_builtin_id
{
	BI_NONE = 0,
	BI_ECHO,
	BI_PWD,
	BI_ENV,
	BI_CD,
	BI_EXPORT,
	BI_UNSET,
	BI_EXIT
}					t_builtin_id;

typedef struct s_builtin_entry
{
	const char		*name;
	t_builtin_id	id;
	t_builtin_fn	fn;
}					t_builtin_entry;

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd,
			"env") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

// cd, export , unset, exit
bool	is_stateful_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

static t_builtin_fn	lookup_builtin_fn(const char *name)
{
	static const t_builtin_entry	table[] = {{"echo", BI_ECHO, builtin_echo},
			{"pwd", BI_PWD, builtin_pwd}, {"env", BI_ENV, builtin_env}, {"cd",
			BI_CD, builtin_cd}, {"export", BI_EXPORT, builtin_export}, {"unset",
			BI_UNSET, builtin_unset}, {"exit", BI_EXIT, builtin_exit}, {NULL,
			BI_NONE, NULL}};
	int								i;

	i = 0;
	if (!name)
		return (NULL);
	while (table[i].name)
	{
		if (ft_strcmp(name, table[i].name) == 0) // exact match!
			return (table[i].fn);
		i++;
	}
	return (NULL);
}

int	execute_builtin(t_ast_command *cmd, t_shell_context *ctx)
{
	t_builtin_fn	fn;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	fn = lookup_builtin_fn(cmd->args[0]);
	if (!fn)
	{
		// 这里理论上不应该发生（外面已经判断 is_builtin）
		// 但为了鲁棒性，返回 1 或 0 都行；通常返回 1 更像“内部错误”
		return (1);
	}
	// 注意：builtin 函数自己负责打印错误并返回对应 status
	// execute_builtin 不要 exit（除了 builtin_exit 自己 exit）
	return (fn(cmd->args, ctx));
}

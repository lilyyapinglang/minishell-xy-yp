#include "../inc/minishell.h"

// buildin, execute in parent

// change directories

/* ✔ Uses getcwd() to track current directory
✔ Falls back to HOME when no args
✔ Uses chdir()
✔ Updates PWD and OLDPWD
✔ Prints errors like bash
✔ Returns success/failure code
*/
// PWD=/home/ylang/code/minishell-github
// OLDPWD=/home/ylang/code/minishell-github

int	builtin_cd(char **argv, t_shell_context *ctx)
{
	const char	*target;
	char		*oldpwd;
	char		*newpwd;
	int			rc;

	if (!argv[1])
	{
		target = env_get_value(ctx->env, "HOME");
		if (!target)
			return (print_msg_n_return(1, "cd", NULL, "HOME not set"));
	}
	else
	{
		target = argv[1];
	}
	oldpwd = getcwd(NULL, 0); // 失败也不一定要中断 cd
	rc = chdir(target);
	if (rc != 0)
	{
		free(oldpwd);
		return (print_errno_n_return(1, "cd", target, errno));
	}
	newpwd = getcwd(NULL, 0);
	if (oldpwd)
		env_set_value(ctx, "OLDPWD", oldpwd, true);
	if (newpwd)
		env_set_value(ctx, "PWD", newpwd, true);
	free(oldpwd);
	free(newpwd);
	return (0);
}

static int	is_valid_ident(const char *str)
{
	int	i;

	// str = null or str = ""
	if (!str || !str[0])
		return (0);
	// str =9 etc
	if (ft_isdigit((unsigned char)str[0]))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!(ft_isalnum((unsigned char)str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **argv, t_shell_context *ctx)
{
	int		i;
	int		status;
	char	*arg;
	char	*equal_sign_loc;
	char	*name;
	char	*val;
	char	*plus_eq;
	int		j;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		// need to sort and print
		print_env(true, ctx);
		return (0);
	}
	while (argv[i]) // check each if --test=100
	{
		arg = argv[i];
		// check - valid options
		if (arg[0] == '-')
		{
			j = 1;
			while (arg[j])
			{
				if (arg[j] != 'f' || arg[j] != 'n' || arg[j] != 'p')
					return (print_msg_n_return(2, "export", arg,
							"invalid option"));
				j++;
			}
		}
		equal_sign_loc = ft_strchr(arg, '=');
		/* Detect NAME+=VALUE safely:
			only when there is an '=' and the char right before '=' is '+' */
		plus_eq = NULL;
		if (equal_sign_loc && equal_sign_loc > arg && equal_sign_loc[-1] == '+')
			plus_eq = equal_sign_loc - 1;
		if (plus_eq)
		{
			/* NAME+=VALUE */
			name = ft_substr(arg, 0, (size_t)(plus_eq - arg));
			val = equal_sign_loc + 1; /* value starts after '=' */
			if (!name || !is_valid_ident(name))
			{
				print_msg_n_return(1, "export", arg, "not a valid identifier");
				status = 1;
			}
			else if (env_append_value(ctx, name, val, true) != 0)
			{
				/* You may choose to print malloc-related error here */
				status = 1;
			}
			free(name);
		}
		else if (equal_sign_loc)
		{
			/* NAME=VALUE */
			name = ft_substr(arg, 0, (size_t)(equal_sign_loc - arg));
			val = equal_sign_loc + 1;
			if (!name || !is_valid_ident(name))
			{
				print_msg_n_return(1, "export", arg, "not a valid identifier");
				status = 1;
			}
			else if (env_set_value(ctx, name, val, true) != 0)
			{
				status = 1;
			}
			free(name);
		}
		else
		{
			/* NAME (export only) */
			if (!is_valid_ident(arg))
			{
				print_msg_n_return(1, "export", arg, "not a valid identifier");
				status = 1;
			}
			else if (env_mark_exported(ctx, arg) != 0)
			{
				status = 1;
			}
		}
		i++;
	}
	return (status);
}

int	builtin_unset(char **argv, t_shell_context *ctx)
{
	int		i;
	int		status;
	char	*arg;
	int		j;

	i = 1;
	status = 0;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		arg = argv[i];
		if (arg[0] == '-')
		{
			j = 1;
			while (arg[j])
			{
				if (arg[j] != 'f' || arg[j] != 'v' || arg[j] != 'n')
					return (print_msg_n_return(2, "unset", arg,
							"invalid option"));
				j++;
			}
		}
		if (is_valid_ident(argv[i]))
			env_unset(ctx, argv[i]);
		i++;
	}
	return (status);
}
int	is_numeric(char *str)
{
	if (str[0] == '+' || str[0] == '-')
	{
		if (check_all_digit(&str[1]))
			return (1);
		else
			return (0);
	}
	else
		return (check_all_digit(str));
}
// exit, but exit which process ???
int	builtin_exit(char **argv, t_shell_context *ctx)
{
	/* no args => exit with last status */
	if (!argv[1])
		exit(ctx->last_status);
	/* non-numeric => error, exit 255 is common in bash;
	if you want strictly your old behavior, keep return 1 + no exit,
	but that differs from bash. Here I won't invent: keep your behavior
	style unless you decide otherwise. */
	if (!is_numeric(argv[1]))
	{
		print_msg_n_return(255, "exit", argv[1], "numeric argument required");
		/* Keep your previous behavior: return 1 (no exit). */
		return (2);
	}
	/* too many args => error,
		DO NOT exit (matches common shell behavior) */
	if (argv[2])
		return (print_msg_n_return(1, "exit", NULL, "too many arguments"));
	exit(ft_atoi(argv[1]));
}

// buildin, execute in child or pipeline
int	builtin_echo(char **argv, t_shell_context *sh_ctx)
{
	char	**strs;
	int		i;

	(void)sh_ctx;
	// echo -n hello world
	// -n means don't add newline charactor at the output
	// argv[0] = echo
	// argv[1] = xxx ?
	if (!argv[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	if (!(ft_strncmp(argv[1], "-n", 2) == 0 && is_only_n(&argv[1][1])))
	{
		// write each char to standard output and add \n
		strs = &argv[1];
		while (*strs)
		{
			ft_putstr_fd(*strs, STDOUT_FILENO);
			if (*(strs + 1)) // not last one, add space
				ft_putchar_fd(' ', STDOUT_FILENO);
			strs++;
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else // if from 1 -> n-1 no -n
	{    // write each char to stardard ouput
		// printf("with -n options \n");
		i = 1;
		while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0
			&& is_only_n(&argv[i][1]))
			i++;
		strs = &argv[i];
		while (*strs)
		{
			ft_putstr_fd(*strs, STDOUT_FILENO);
			if (*(strs + 1)) // not last one, add space
				ft_putchar_fd(' ', STDOUT_FILENO);
			strs++;
		}
	}
	return (0);
}

// print working directory
int	builtin_pwd(char **argv, t_shell_context *ctx)
{
	char	*pwd;

	(void)argv;
	(void)ctx;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (print_errno_n_return(1, "pwd", NULL, errno));
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

t_ast	*new_ast_command(char **args)
{
	t_ast	*node;

	// 1. Allocate memory for the generic AST node
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	// 2. Set the type tag so we know which union member to access later
	node->type = AST_COMMAND;
	// 3. Initialize the specific union data
	node->u_data.command.args = args;
	// 4. Set default flags
	node->is_expanded = false;
	return (node);
}

int	builtin_env(char **argv, t_shell_context *ctx)
{
	t_ast	*cmd_node;
	int		status;

	if (!argv[1])
	{
		print_env(false, ctx);
		return (0);
	}
	else // env what  , run what with env ,
	{
		cmd_node = new_ast_command(&argv[1]);
		if (!cmd_node)
			return (EXIT_FAILURE);
		status = execute_command(cmd_node, RUN_IN_CHILD, ctx);
		free(cmd_node);
		return (status);
	}
}

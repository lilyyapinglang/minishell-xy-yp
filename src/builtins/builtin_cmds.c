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

static int	is_valid_ident(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (ft_isdigit((unsigned char)s[0]))
		return (0);
	i = 0;
	while (s[i])
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
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
	char	*eq;
	char	*name;
	char	*val;
	char	*plus_eq;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_env(true, ctx);
		return (0);
	}
	while (argv[i])
	{
		arg = argv[i];
		eq = ft_strchr(arg, '=');
		/* Detect NAME+=VALUE safely:
			only when there is an '=' and the char right before '=' is '+' */
		plus_eq = NULL;
		if (eq && eq > arg && eq[-1] == '+')
			plus_eq = eq - 1;
		if (plus_eq)
		{
			/* NAME+=VALUE */
			name = ft_substr(arg, 0, (size_t)(plus_eq - arg));
			val = eq + 1; /* value starts after '=' */
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
		else if (eq)
		{
			/* NAME=VALUE */
			name = ft_substr(arg, 0, (size_t)(eq - arg));
			val = eq + 1;
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
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		if (!is_valid_ident(argv[i]))
		{
			print_msg_n_return(1, "unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
		{
			env_unset(ctx, argv[i]);
		}
		i++;
	}
	return (status);
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
	if (!check_all_digit(argv[1]))
	{
		print_msg_n_return(255, "exit", argv[1], "numeric argument required");
		/* Keep your previous behavior: return 1 (no exit). */
		return (2);
	}
	/* too many args => error, DO NOT exit (matches common shell behavior) */
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
		while (ft_strncmp(argv[i], "-n", 2) == 0 && is_only_n(&argv[i][1]))
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

int	builtin_env(char **argv, t_shell_context *ctx)
{
	(void)argv;
	print_env(false, ctx);
	return (0);
}

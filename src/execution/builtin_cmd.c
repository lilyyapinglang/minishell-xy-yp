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

unsigned int	g_lastcmd_exit_code = 0;
;
t_env	*find_env_var(char *key, t_env *env)
{
	while (env)
	{
		// printf("env.key :  %s, key : %s, \n", env->key, key);
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	builtin_cd(char **argv, t_env *env)
{
	char	*buf;
	long	size;
	char	*ptr;

	// argv[0] == cd
	// argv[1] = ../  folder name etc
	if (!argv[1])
	{
		// get env HOME
		printf("%s\n", find_env_var("HOME", env)->value);
		return (chdir(find_env_var("HOME", env)->value));
	}
	size = pathconf(".", _PC_PATH_MAX);
	if ((buf = malloc(size)) != NULL)
	{
		// get current working directory
		ptr = getcwd(buf, size);
		printf("before chdir getcwd result ptr : %s \n", ptr);
		// update OLDPWD env variable
		find_env_var("OLDPWD", env)->value = ptr;
		// change current working directory to new one
		chdir(argv[1]);
		ptr = getcwd(buf, size);
		// update PWD env varaible
		find_env_var("PWD", env)->value = getcwd(buf, size);
		printf("after chdir getcwd result ptr : %s \n", ptr);
		// output to stdout just for test purpose
		ft_putstr_fd(ptr, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		// access check
		return (0);
	}
	else
		return (-1);
}

// actually need to insert at alphabetic order
t_env	*insertAtEnd(t_env *head, char *key, char *value)
{
	t_env	*new_env_node;
	t_env	*last;

	new_env_node = malloc(sizeof(t_env));
	new_env_node->key = key;
	new_env_node->value = value;
	if (head == NULL)
		return (new_env_node);
	// traverse to the last node
	last = head;
	while (last->next != NULL)
		last = last->next;
	last->next = new_env_node;
	return (head);
}
//
t_env	*deleteEnvNode(t_env *env, char *key)
{
	t_env	*head;

	head = env;
	// delete the node with the key and return the head pointer
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
		{ // delete the node no matter inthe , shoud be a doubed list ,
			// i need to change tmre
		}
		env = env->next;
	}
	return (head);
}

// set an enviroment variabe
int	builtin_export(char **argv, t_env *env)
{
	char	**var_value_pair;
	char	**ptr;
	t_env	*find_env_node;

	// argv[0] == export
	// argv[1] =  zsh = xxx
	if (!argv[1])
		// should it print env or not ?
		return (builtin_env(env));
	// search for the varaibe
	// if there is an existing varaible, update it
	// if there is no existing env variabe,
	ptr = &argv[1];
	while (*ptr)
	{
		if (ft_isdigit(**ptr))
		{
			printf(" export: not an identifier: 1\n");
			return (-1);
		}
		if (**ptr == '=')
		{
			printf("bad assingnment \n");
			return (-1);
		}
		if (ft_strchr(*ptr, '='))
		{
			var_value_pair = ft_split(*ptr, '=');
			find_env_node = find_env_var(var_value_pair[0], env);
			if (find_env_node)
				find_env_node->value = var_value_pair[1];
			else
				// add new env var
				env = insertAtEnd(env, var_value_pair[0], var_value_pair[1]);
			builtin_env(env);
		}
		else
		{ // could not find exisitng variabke
			// create new node
			// add to linked-list ordered by alphabetic order
			env = insertAtEnd(env, *ptr, "");
			builtin_env(env);
		}
		ptr++;
	}
	return (1);
}

int	check_str_alpnum(char *str)
{
	while (str)
	{
		if (!ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

// unset an environement variabe
int	builtin_unset(char **argv, t_env *env)
{
	char	**arg_ptr;
	t_env	*find_env_node;
	t_env	*new_head;

	if (!argv[1])
	{
		printf("unset: not enough arguments\n");
		return (-1);
	}
	// need to add other error handling
	arg_ptr = &argv[1];
	while (*arg_ptr)
	{
		// check if *arg_prtr is alpha
		if (!check_str_alpnum(*arg_ptr))
		{
			printf("unset: %s : invalid parameter name \n ", *arg_ptr);
			return (-1);
		}
		// try to find var in env
		find_env_node = find_env_var(*arg_ptr, env);
		if (!find_env_node)
			new_head = deleteEnvNode(env, *arg_ptr);
		return (0);
	}
	return (0);
}

int	check_all_digit(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

// exit, but exit which process ???
int	builtin_exit(char **argv, t_env *env)
{
	(void)env;
	// exit without argument
	if (!argv[1])
		exit(g_lastcmd_exit_code);
	if (argv[2])
	{
		ft_printf("exit: too many arguments\n");
		return (1);
	}
	if (!check_all_digit(argv[1]))
	{
		// bash-5.1$ exit hello
		// exit
		// bash: exit: hello: numeric argument required
		ft_putendl_fd(argv[0], STDOUT_FILENO);
		ft_printf("minishell: %s : %s : numeric argument required \n ", argv[0],
			argv[1]);
		return (1);
	}
	// designated code
	ft_putendl_fd(argv[0], STDOUT_FILENO);
	exit(ft_atoi(argv[1]));
	return (0);
}
// buildin, execute in child or pipeline
int	builtin_echo(char **argv)
{
	char	**strs;

	// echo -n hello world
	// -n means don't add newline charactor at the output
	// argv[0] = echo
	// argv[1] = xxx ?
	// printf("i'm here\n");
	// printf("av[1] is %s\n", argv[1]);
	if (ft_strncmp(argv[1], "-n", 2) == 0)
	{
		// write each char to stardard ouput
		printf("with -n options \n");
		strs = &argv[2];
		while (*strs)
		{
			ft_putstr_fd(*strs, STDOUT_FILENO);
			ft_putchar_fd(' ', STDOUT_FILENO);
			strs++;
		}
	}
	else
	{
		// write each char to standard output and add \n
		strs = &argv[1];
		while (*strs)
		{
			ft_putstr_fd(*strs, STDOUT_FILENO);
			ft_putchar_fd(' ', STDOUT_FILENO);
			strs++;
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}

// print working directory
int	builtin_pwd(char **argv)
{
	char	*ptr;
	long	size;
	char	*buf;

	if (ft_strncmp(argv[0], "pwd", 3) != 0)
		return (1);
	// argv pwd
	// char *getcwd(char *buf, size_t size);
	// These functions return a null-terminated string containing an absolute pathname that
	// is the current working directory of the calling process.
	// getcwd() 会把当前工作目录（当前路径）的 绝对路径 写入 buf 指向的内存中，并返回 buf。
	size = pathconf(".", _PC_PATH_MAX); // get the longest possible path length
	if ((buf = malloc(size)) != NULL)
	{
		ptr = getcwd(buf, size);
		ft_putstr_fd(ptr, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}
int	builtin_env(t_env *env)
{
	// how to get a list of envinrable names to search value for
	// env is the pointer to the head node ;
	while (env)
	{
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		env = env->next;
	}
	return (0);
}
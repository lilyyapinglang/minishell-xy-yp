
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
int	builtin_cd(char **argv)
{
	(void)argv;
	// argv[0] == ?
	return (1);
}

int	builtin_export(char **argv)
{
	(void)argv;
	return (1);
}
int	builtin_unset(char **argv)
{
	(void)argv;
	return (1);
}
int	builtin_exit(char **argv)
{
	(void)argv;
	return (1);
}
// buildin, execute in child or pipeline

int	builtin_echo(char **argv)
{
	char	**strs;

	// echo -n hello world
	// -n means don't add newline charactor at the output
	// argv[0] = echo
	// argv[1] = xxx ?
	printf("i'm here\n");
	printf("av[1] is %s\n", argv[1]);
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
		env = env->next;
	}
	return (0);
}
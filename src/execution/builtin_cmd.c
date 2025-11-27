
#include "../inc/minishell.h"

// buildin, execute in parent

// change directories

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
	char	*cwd;

	// argv pwd
	// char *getcwd(char *buf, size_t size);
	// These functions return a null-terminated string containing an absolute pathname that
	// is the current working directory of the calling process.
	cwd = getcwd();
	ft_putstr_fd(cwd, STDOUT_FILENO);
	return (0);
}
int	builtin_env(char **env)
{
	// getenv
	// how to get a listo f envinrable names to search value for
	// get an environment variable
	// char *getenv(const char *name);
	char *env;
	char *name;

	while (*env)
	{
	}
	char return (0);
}

#include "../inc/minishell.h"

// buildin, execute in parent

int	builtin_cd(char **argv)
{
	(void)argv;
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
	(void)argv;
	return (1);
}
int	builtin_pwd(char **argv)
{
	(void)argv;
	return (1);
}
int	builtin_env(char **argv)
{
	(void)argv;
	return (1);
}
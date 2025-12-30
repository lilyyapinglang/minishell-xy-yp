#include "../inc/minishell.h"
#include <stdio.h> // optional for perror
#include <stdlib.h>
#include <string.h> // for strdup (or use ft_strdup)

// cat a | grep hi | wc -l
/*
nb_cmds = 3

cmd[0]:
  argv = ["cat", "a", NULL]
  redirs = []

cmd[1]:
  argv = ["grep", "hi", NULL]
  redirs = []

cmd[2]:
  argv = ["wc", "-l", NULL]
  redirs = []

*/

static char	**dup_argv(const char *const tokens[])
{
	size_t	n;
	char	**argv;

	n = 0;
	while (tokens[n])
		n++;
	argv = (char **)calloc(n + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	for (size_t i = 0; i < n; i++)
	{
		argv[i] = strdup(tokens[i]); // replace with ft_strdup if you want
		if (!argv[i])
		{
			// rollback
			for (size_t j = 0; j < i; j++)
				free(argv[j]);
			free(argv);
			return (NULL);
		}
	}
	argv[n] = NULL;
	return (argv);
}

static t_cmd	*new_cmd_from_tokens(const char *const tokens[])
{
	t_cmd	*cmd;

	cmd = (t_cmd *)calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = dup_argv(tokens);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	append_cmd(t_cmd_table *t, t_cmd *node)
{
	t_cmd	*cur;

	if (!t->cmds)
	{
		t->cmds = node;
	}
	else
	{
		cur = t->cmds;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
	t->cmds_count++;
}

/* ---------- test builder ---------- */

t_cmd_table	*build_fake_cmd_table_for_tests(void)
{
	t_cmd_table	*t;
	const char	*cmd1[] = {"cat", "a.txt", NULL};
	const char	*cmd2[] = {"grep", "hi", NULL};
	const char	*cmd3[] = {"wc", "-l", NULL};
	t_cmd		*c1;
	t_cmd		*c2;
	t_cmd		*c3;

	t = (t_cmd_table *)calloc(1, sizeof(t_cmd_table));
	if (!t)
		return (NULL);
	// Example pipeline: cat a.txt | grep hi | wc -l
	c1 = new_cmd_from_tokens(cmd1);
	c2 = new_cmd_from_tokens(cmd2);
	c3 = new_cmd_from_tokens(cmd3);
	if (!c1 || !c2 || !c3)
	{
		// free whatever succeeded (use free_cmd_table below)
		// easiest: temporarily attach and free
		if (c1)
		{
			append_cmd(t, c1);
		}
		if (c2)
		{
			append_cmd(t, c2);
		}
		if (c3)
		{
			append_cmd(t, c3);
		}
		// caller can call free_cmd_table(t),
		// but let's do it here to avoid leaks:
		// (we'll implement free_cmd_table below and call it)
	}
	append_cmd(t, c1);
	append_cmd(t, c2);
	append_cmd(t, c3);
	return (t);
}

/* ---------- destructor ---------- */

void	free_cmd_table(t_cmd_table *t)
{
	t_cmd	*cur;
	t_cmd	*next;
	t_redir	*r;
	t_redir	*rn;

	if (!t)
		return ;
	cur = t->cmds;
	while (cur)
	{
		next = cur->next;
		if (cur->argv)
		{
			for (size_t i = 0; cur->argv[i]; i++)
				free(cur->argv[i]);
			free(cur->argv);
		}
		// redirects (unused in tests for now, but safe to free)
		r = cur->redirects;
		while (r)
		{
			rn = r->next;
			free(r->file);
			free(r);
			r = rn;
		}
		free(cur);
		cur = next;
	}
	free(t);
}

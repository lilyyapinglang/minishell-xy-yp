/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:17:13 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/13 16:34:53 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static size_t	hd_value_len_at(const char *line, size_t i,
	t_shell_context *sh, const char *st)
{
	size_t	n;
	char	*name;
	char	*val;

	n = hd_var_name_len(line + i + 1);
	if (n == 0)
		return (1);
	if (n == 1 && line[i + 1] == '?')
		return (ft_strlen(st));
	name = hd_name_sub(line, i, n, sh);
	if (!name)
		return ((size_t) - 1);
	val = env_get_value(sh->env, name);
	free(name);
	if (!val)
		return (0);
	return (ft_strlen(val));
}

static size_t	hd_compute_len(const char *line, t_shell_context *sh,
								const char *st)
{
	size_t	i;
	size_t	len;
	size_t	add;

	i = 0;
	len = 0;
	while (line[i])
	{
		if (line[i] != '$')
		{
			len++;
			i++;
		}
		else
		{
			add = hd_value_len_at(line, i, sh, st);
			if (add == (size_t) - 1)
				return ((size_t) - 1);
			len += add;
			i += 1 + hd_var_name_len(line + i + 1);
		}
	}
	return (len);
}

static void	hd_fill(char *out, const char *line,
	t_shell_context *sh, const char *st)
{
	size_t			i;
	t_hd_fillctx	ctx;

	i = 0;
	ctx.out = out;
	ctx.j = 0;
	ctx.sh = sh;
	ctx.st = st;
	while (line[i])
	{
		if (line[i] != '$')
			ctx.out[ctx.j++] = line[i++];
		else
		{
			hd_fill_dollar(&ctx, line + i);
			i += 1 + hd_var_name_len(line + i + 1);
		}
	}
	ctx.out[ctx.j] = '\0';
}

/* main heredoc expander */

char	*heredoc_expand_line(const char *line, t_shell_context *sh)
{
	char	*st;
	size_t	out_len;
	char	*out;

	if (!line)
		return (s_alloc(ft_strdup(""), ALLOC_UNTRACKED, sh));
	st = s_alloc(ft_itoa(sh->last_status), ALLOC_UNTRACKED, sh);
	if (!st)
		return (NULL);
	out_len = hd_compute_len(line, sh, st);
	if (out_len == (size_t)-1)
		return (free(st), NULL);
	out = calloc_s(out_len + 1, sizeof(char), ALLOC_UNTRACKED, sh);
	if (!out)
		return (free(st), NULL);
	hd_fill(out, line, sh, st);
	free(st);
	return (out);
}

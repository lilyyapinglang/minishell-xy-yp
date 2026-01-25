/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:17:13 by xuewang           #+#    #+#             */
/*   Updated: 2026/01/25 15:52:59 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	hd_is_var_first(int c)
{
	return (c == '_' || ft_isalpha((unsigned char)c));
}

static int	hd_is_var_char(int c)
{
	return (c == '_' || ft_isalnum((unsigned char)c));
}


/* After '$', valid names:
    '?' (special) => len 1
    legal word=> len
    else => 0 (treat '$' literally) 
*/

static size_t	hd_var_name_len(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (0);
	if (*s == '?')
		return (1);
	if (!hd_is_var_first(*s))
		return (0);
	i = 1;
	while (s[i] && hd_is_var_char(s[i]))
		i++;
	return (i);
}

static char	*hd_name_sub(const char *line, size_t i, size_t n, t_shell_context *sh)
{
	char	*tmp;

	tmp = ft_substr(line, (unsigned int)(i + 1), n);
	return (s_alloc(tmp, ALLOC_UNTRACKED, sh));
    
}
/*Compute how many characters the VAR(thing after $) will expand into*/

static size_t	hd_value_len(const char *line, size_t i,
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
		return ((size_t)-1);
	val = env_get_value(sh->env, name);
	free(name);
	if (!val)
		return (0);
	return (ft_strlen(val));
}

/*
*hd_compute_len:
* Compute the final expanded length for this line. return final length
*(to malloc once for the output string
*
*/

static size_t	hd_compute_len(const char *line, t_shell_context *sh, const char *st)
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
			continue ;
		}
		add = hd_value_len(line, i, sh, st);
		if (add == (size_t)-1)
			return ((size_t)-1);
		len += add;
		i += 1 + hd_var_name_len(line + i + 1);
		if (hd_var_name_len(line + i) == 0 && line[i - 1] == '$')
			;
	}
	return (len);
}

static size_t	hd_copy_str(char *out, size_t j, const char *s)
{
	size_t	k;

	k = 0;
	while (s && s[k])
		out[j++] = s[k++];
	return (j);
}
/*handle $*/
static size_t	hd_fill_dollar(char *out, size_t j, const char *line,
				t_shell_context *sh, const char *st)
{
	size_t	n;
	char	*name;
	char	*val;

	n = hd_var_name_len(line + 1);
	if (n == 0)
		return (out[j++] = '$', j);
	if (n == 1 && line[1] == '?')
		return (hd_copy_str(out, j, st));
	name = hd_name_sub(line, 0, n, sh);
	if (!name)
		return (j);
	val = env_get_value(sh->env, name);
	free(name);
	if (val)
		j = hd_copy_str(out, j, val);
	return (j);
}

/*fill things in final output*/
static void	hd_fill(char *out, const char *line, t_shell_context *sh, const char *st)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] != '$')
			out[j++] = line[i++];
		else
		{
			j = hd_fill_dollar(out, j, line + i, sh, st);
			i += 1 + hd_var_name_len(line + i + 1);
			if (hd_var_name_len(line + i) == 0 && line[i - 1] == '$')
				;
		}
	}
	out[j] = '\0';
}

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

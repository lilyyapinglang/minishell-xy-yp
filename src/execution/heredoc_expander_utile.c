/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander_utile.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyan_wang <xueyan_wang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 21:27:26 by xueyan_wang       #+#    #+#             */
/*   Updated: 2026/02/10 22:24:27 by xueyan_wang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** first == 1 : first char after '$' => '_' or alpha
** first == 0 : subsequent chars     => '_' or alnum
*/
static int	hd_is_var_ok(int c, int first)
{
	if (c == '_')
		return (1);
	if (first)
		return (ft_isalpha((unsigned char)c));
	return (ft_isalnum((unsigned char)c));
}

size_t	hd_var_name_len(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (0);
	if (*s == '?')
		return (1);
	if (!hd_is_var_ok(*s, 1))
		return (0);
	i = 1;
	while (s[i] && hd_is_var_ok(s[i], 0))
		i++;
	return (i);
}

char	*hd_name_sub(const char *line, size_t i, size_t n, t_shell_context *sh)
{
	char	*tmp;
	char	*ret;

	tmp = ft_substr(line, (unsigned int)(i + 1), n);
	if (!tmp)
		return (NULL);
	ret = s_alloc(tmp, ALLOC_UNTRACKED, sh);
	return (ret);
}

size_t	hd_copy_str(char *out, size_t j, const char *s)
{
	size_t	k;

	k = 0;
	while (s && s[k])
		out[j++] = s[k++];
	return (j);
}

size_t	hd_fill_dollar(char *out, size_t j, const char *line,
	t_shell_context *sh, const char *st)
{
	size_t	n;
	char	*name;
	char	*val;

	n = hd_var_name_len(line + 1);
	if (n == 0)
	{
		out[j++] = '$';
		return (j);
	}
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

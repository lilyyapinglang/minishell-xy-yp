/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:29:17 by ylang             #+#    #+#             */
/*   Updated: 2025/05/15 00:10:48 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_char2remove(char a, char *chars2remove)
{
	while (*chars2remove)
	{
		if (*chars2remove == a)
			return (1);
		chars2remove++;
	}
	return (0);
}

static void	get_trim_range(const char *s1, const char *set, char **start,
		char **end)
{
	char	*ptr;
	char	*charset;
	int		len_s1;

	charset = (char *)set;
	len_s1 = ft_strlen(s1);
	ptr = (char *)s1;
	while (*ptr && ft_is_char2remove(*ptr, charset))
		ptr++;
	*start = ptr;
	ptr = (char *)s1 + len_s1 - 1;
	while (ptr > *start && ft_is_char2remove(*ptr, charset))
		ptr--;
	*end = ptr;
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*start;
	char	*end;
	char	*new_str;
	char	*write_ptr;
	size_t	len;

	if (!s1)
		return (NULL);
	if (!*s1 || !set || !*set)
		return (ft_strdup(s1));
	get_trim_range(s1, set, &start, &end);
	if (*start == '\0')
		return (ft_strdup(""));
	len = end - start + 1;
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	write_ptr = new_str;
	while (start <= end)
		*write_ptr++ = *start++;
	*write_ptr = '\0';
	return (new_str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:23:38 by ylang             #+#    #+#             */
/*   Updated: 2026/03/03 16:46:17 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	const char	*ptr;

	ptr = str;
	while (*ptr)
	{
		ptr++;
	}
	return (ptr - str);
}

char	*ft_strdup(const char *str)
{
	char	*new_str;
	int		len;
	char	*ptr;

	len = ft_strlen(str);
	new_str = (char *)malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	ptr = new_str;
	while (*str)
	{
		*ptr = *str;
		str++;
		ptr++;
	}
	*ptr = '\0';
	if (!*str)
		*ptr = '\0';
	return (new_str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*writestr;
	char	*substr;
	size_t	len_s;
	size_t	substr_len;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (!*s || !len || start >= len_s)
		return (ft_strdup(""));
	if (len_s - start >= len)
		substr_len = len;
	else
		substr_len = len_s - start;
	substr = (char *)malloc((substr_len + 1) * sizeof(char));
	if (!substr)
		return (substr);
	writestr = substr;
	while (substr_len)
	{
		*writestr++ = *(s + start++);
		substr_len--;
	}
	*writestr = '\0';
	return (substr);
}

char	*ft_strchr(const char *str, int searchedChar)
{
	char	*ptr;
	char	c;

	ptr = (char *)str;
	c = (unsigned char)searchedChar;
	while (*ptr)
	{
		if (*ptr == c)
			return (ptr);
		ptr++;
	}
	if (c == '\0')
		return (ptr);
	return ((void *)0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ptr;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ptr = str;
	while (*s1)
		*ptr++ = *s1++;
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (str);
}

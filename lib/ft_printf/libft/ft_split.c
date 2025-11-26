/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:28:28 by ylang             #+#    #+#             */
/*   Updated: 2025/05/14 22:11:03 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_substrs(char *start, char *end)
{
	size_t	len;
	size_t	i;
	char	*substr;

	len = (size_t)(end - start);
	substr = malloc(len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = start[i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

static int	count_segments(const char *str, char c)
{
	char	*ptr;
	int		seg_count;

	ptr = (char *)str;
	seg_count = 0;
	while (*ptr)
	{
		while (*ptr && *ptr == c)
			ptr++;
		if (*ptr)
		{
			seg_count++;
			while (*ptr && *ptr != c)
				ptr++;
		}
	}
	return (seg_count);
}

static int	handle_split_error(char **sub_strs, int i)
{
	while (i > 0)
	{
		i--;
		free(sub_strs[i]);
	}
	free(sub_strs);
	return (-1);
}

static int	fill_split(char *ptr, char c, char **sub_strs)
{
	char	*seg_start;
	int		i;

	i = 0;
	while (*ptr)
	{
		while (*ptr && *ptr == c)
			ptr++;
		if (*ptr)
		{
			seg_start = ptr;
			while (*ptr && *ptr != c)
				ptr++;
			sub_strs[i] = ft_substrs(seg_start, ptr);
			if (!sub_strs[i])
				return (handle_split_error(sub_strs, i));
			i++;
			while (*ptr && *ptr == c)
				ptr++;
		}
	}
	sub_strs[i] = NULL;
	return (0);
}

char	**ft_split(const char *str, char c)
{
	char	**sub_strs;
	char	*ptr;
	int		seg_count;

	if (!str)
		return (NULL);
	seg_count = 0;
	seg_count = count_segments(str, c);
	sub_strs = (char **)malloc((seg_count + 1) * sizeof(char *));
	if (!sub_strs)
		return (NULL);
	ptr = (char *)str;
	if (fill_split(ptr, c, sub_strs) == -1)
		return (NULL);
	return (sub_strs);
}

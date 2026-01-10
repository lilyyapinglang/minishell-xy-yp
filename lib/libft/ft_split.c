/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:36:36 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/15 14:43:04 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
#include<stdio.h>

int	str_count(char const *str, char c)
{
	size_t	i;
	size_t	strcount;
	size_t	trigger;

	i = 0;
	strcount = 0;
	trigger = 1;
	while (str[i])
	{
		if (str[i] == c)
			trigger = 1;
		if (trigger == 1 && str[i] != c)
		{
			strcount++;
			trigger = 0;
		}
		i++;
	}
	return (strcount);
}

char	*cpy_str_to_dest(char const *str, int start, int end)
{
	int		i;
	char	*dest;

	dest = (char *)malloc(sizeof (char) * (end - start + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while ((start + i) < end)
	{
		dest[i] = str[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	**free_split(char **arr, size_t j)
{
	while (j > 0)
		free(arr[--j]);
	free(arr);
	return (NULL);
}

char	**sub_split(char const *s, char c, char **arr)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (start < i)
		{
			arr[j] = cpy_str_to_dest(s, start, i);
			if (!arr[j])
				return (free_split(arr, j));
			j++;
		}
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (str_count(s, c) + 1));
	if (!arr)
		return (NULL);
	return (sub_split(s, c, arr));
}
/*
int	main (int ac, char** av)
{
	char**	arr;
	int		i;
	int		i;
	char**	arr;
	ac = 3;
	i = 0;
	arr = ft_split(av[1], (*av[2]));
	printf("%s\n", arr[1]);
	printf("%d\n",str_count(av[1], (*av[2])));
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}
*/

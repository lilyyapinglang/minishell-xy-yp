/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:52:01 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 14:50:28 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//#include<stdio.h>
//#include<string.h>
/*
int	ft_strlen(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
*/
int	test_set(char c, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/*int	set_detect(char const *s1, char const *set)
{
	size_t	i;
	size_t	time;


	i = 0;
	time = 0;
	while (s1[i])
	{
		if (test_set(s1[i], set) == 1)
			time++;
		i++;
	}
	return (time);
}
*/

char	*put_str_to_dest(char const *s1, size_t start, size_t end)
{
	char	*dest;
	size_t	i;

	if (start > end)
	{
		dest = malloc (1);
		if (!dest)
			return (NULL);
		dest[0] = '\0';
		return (dest);
	}
	dest = malloc(sizeof(char) * (end - start + 2));
	if (!dest)
		return (NULL);
	i = 0;
	while (start <= end)
		dest[i++] = s1[start++];
	dest[i] = '\0';
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while ((s1[start]) && (test_set (s1[start], set) != 0))
		start++;
	len = ft_strlen(s1);
	if (len == 0)
		end = 0;
	else
		end = len - 1;
	if (start == end)
		return (ft_strdup(""));
	while ((end > start) && (s1[end]) && (test_set (s1[end], set) != 0))
		end--;
	return (put_str_to_dest(s1, start, end));
}
/*
end = strlen(s1);
int	main (int ac, char** av)
{	
	ac = 3;
	//printf ("%d\n", num_set(av[1], av[2]));
	printf ("%s\n",ft_strtrim(av[1], av[2]));
	//printf ("%s\n", av[1]);
	return (0);
}
*/

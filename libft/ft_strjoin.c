/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:12:24 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 19:37:42 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include<stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[j++] = s1[i++];
	}
	i = 0;
	while (s2[i])
	{
		dest[j++] = s2[i++];
	}
	dest[j] = '\0';
	return (dest);
}
/*
int	main (int ac, char** av)
{
	char*	dest;
	if (ac == 3)
	{
		dest = ft_strjoin(av[1], av[2]);
		printf("%s\n", dest);
	}
	return (0);
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 21:03:49 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 19:33:17 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
#include<stdlib.h>
#include<stdio.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (little[0] == '\0')
		return ((char *)big);
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while ((i + j) < len
			&& big[i + j] && little[j]
			&& big[i + j] == little[j])
		{
			j++;
		}
		if (little[j] == '\0')
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}
/*
int	main()
{
	const char*	big = "helloworld";
	const char*	little = "world";
	size_t	len = 11;

	char*	result = ft_strnstr(big ,little, len);
	printf ("%s\n", result);
}
*/

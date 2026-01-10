/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:48 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 18:55:02 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

//#define ft_strlen strlen

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	len_d;
	size_t	len_s;

	len_d = ft_strlen(dest);
	len_s = ft_strlen(src);
	if (len_d >= size)
		return (len_s + size);
	i = 0;
	while (src[i] && (len_d + i < size - 1))
	{
		dest[len_d + i] = src[i];
		i++;
	}
	dest[len_d + i] = '\0';
	return (len_d + len_s);
}
/*
int	main ()
{
	char	buf[7] = "hello";
	const char	*src = "world";
	size_t	result;

	result = ft_strlcat(buf, src, sizeof(buf));

	printf("ft_strlcat result : \"%s\"\n", buf);
	printf("Return value      : %zu\n", result);
}
*/

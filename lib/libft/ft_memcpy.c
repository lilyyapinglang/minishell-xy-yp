/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:33:04 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 17:30:42 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//#include<string.h>
//#include<stdlib.h>
//#include<stdio.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src && n != 0)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/*
int	main ()
{
	char	*dest;
	const char	*src;
//	int	n;
	//char	*R1;
	char	*R2;

	dest = NULL;
	src = NULL;

	//R1 = ft_memcpy(dest, src, 3);
	R2 = memcpy(dest, src, 3);

	//printf("%s\n",R1);
	printf("%s\n",R2);
	return (0);
}
*/

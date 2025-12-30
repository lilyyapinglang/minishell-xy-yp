/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:26:04 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 18:15:55 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include"libft.h"
//#include<string.h>
//#include<stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	size_t	i;

	if ((unsigned char *)dest == (unsigned char *)src || len == 0)
		return (dest);
	if ((unsigned char *)dest < (unsigned char *)src)
	{
		i = 0;
		while (i < len)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		i = len;
		while (i > 0)
		{
			i--;
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		}
	}
	return (dest);
}
/*
int	main()
{
	char	src[20] = "hello world";
	char	buf1[50];
	char	buf2[50];
	char	buf3[20] = "11111";
	char	buf4[20] = "11111";
	memmove(buf1, src, 12);
	ft_memmove(buf2, src, 12);

	printf("libc memmove: %s\n", buf1);
	printf("ft_memmove  : %s\n", buf2);
	
	memmove(buf3 + 1, buf3, 4);      // to test the overlapping
	ft_memmove(buf4 + 1, buf4, 4);
	
	printf("libc memmove (overlap): %s\n", buf1);
	printf("ft_memmove   (overlap): %s\n", buf2);

}
*/

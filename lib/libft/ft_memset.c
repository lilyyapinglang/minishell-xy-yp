/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:49:04 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 16:03:35 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include<string.h>
//#include<stdlib.h>
//#include<stdio.h>
#include"libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*un_s;
	unsigned char	un_c;
	size_t			i;

	un_s = (unsigned char *)s;
	un_c = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		un_s[i] = un_c;
		i++;
	}
	return (s);
}
/*
void print_bytes(const void *ptr, size_t n)
{
    const unsigned char *p = ptr;
    size_t	i;

    i = 0;
    while (i < n)
    {
        printf("%02X ", p[i]);
	i++;
    }
    printf("\n");
}

int main(void)
{
	void	*ptr;
	ptr = NULL;
    char *buf1 = malloc(10);
    char *buf2 = malloc(10);

    if (!buf1 || !buf2)
        return (1);

    //memset(buf1, 'A', 10);
    //ft_memset(buf2, 'A', 10);
    memset(ptr, 'A', 10);
    ft_memset(ptr, 'A', 10);

    printf("libc memset  : ");
    print_bytes(buf1, 10);

    printf("ft_memset    : ");
    print_bytes(buf2, 10);
    
    if (memcmp(buf1, buf2, 10) == 0)
        printf("✅ OK: ft_memset matches libc\n");
    else
        printf("❌ FAIL: ft_memset differs from libc\n");

    free(buf1);
    free(buf2);
    return 0;
}
*/

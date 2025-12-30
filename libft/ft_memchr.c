/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:15:31 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/06 22:18:04 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*un_s;
	unsigned char	un_c;	

	i = 0;
	un_s = (unsigned char *)s;
	un_c = (unsigned char)c;
	while (i < n)
	{
		if (un_s[i] == un_c)
			return ((void *)(s + i));
		i++;
	}
	return (0);
}
/*
int	main(int ac, char** av)
{
	ac = 2;
*/

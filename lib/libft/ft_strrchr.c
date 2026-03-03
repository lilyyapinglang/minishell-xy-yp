/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyan_wang <xueyan_wang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:23:32 by xuewang           #+#    #+#             */
/*   Updated: 2026/03/03 09:21:51 by xueyan_wang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			len;
	unsigned char	find;

	find = c;
	len = ft_strlen(s);
	if (s[len] == find)
		return ((char *)s + len);
	while (len > 0)
	{
		len--;
		if (s[len] == find)
			return ((char *)s + len);
	}
	return (NULL);
}

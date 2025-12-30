/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:23:32 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/06 21:02:35 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

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

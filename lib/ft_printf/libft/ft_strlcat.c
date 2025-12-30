/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:28:41 by ylang             #+#    #+#             */
/*   Updated: 2025/05/14 22:37:31 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	char		*ptr_dest;
	const char	*ptr_src;
	size_t		len_dst;
	size_t		len_src;
	size_t		remaining2cpy;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	ptr_dest = dst;
	ptr_src = src;
	if (dstsize <= len_dst)
		return (dstsize + len_src);
	remaining2cpy = dstsize - 1 - len_dst;
	while (*ptr_src && remaining2cpy)
	{
		*(ptr_dest + len_dst) = *ptr_src;
		ptr_src++;
		ptr_dest++;
		remaining2cpy--;
	}
	*(ptr_dest + len_dst) = '\0';
	return (len_dst + len_src);
}

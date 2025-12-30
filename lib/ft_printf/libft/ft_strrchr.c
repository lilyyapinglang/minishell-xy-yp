/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:29:14 by ylang             #+#    #+#             */
/*   Updated: 2025/05/14 23:30:17 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int searchedChar)
{
	const char	*end;
	char		c;
	size_t		len;

	len = ft_strlen(str);
	end = str + len;
	c = (char)searchedChar;
	while (end > str)
	{
		if (*end == c)
			return ((char *)end);
		end--;
	}
	if (*end == c)
		return ((char *)end);
	return (NULL);
}

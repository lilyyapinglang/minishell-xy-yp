/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:29:11 by ylang             #+#    #+#             */
/*   Updated: 2025/05/14 23:30:14 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	match_substring(const char *big_ptr, const char *little,
		size_t remain)
{
	const char	*little_ptr;

	little_ptr = little;
	while (*big_ptr && *little && *big_ptr == *little_ptr && remain)
	{
		big_ptr++;
		little_ptr++;
		remain--;
	}
	return (!*little_ptr);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	const char	*big_ptr;
	size_t		remain;

	big_ptr = big;
	remain = len;
	if (!*little)
		return ((char *)big);
	while (*big_ptr && remain > 0)
	{
		if (match_substring(big_ptr, little, remain))
			return ((char *)big_ptr);
		big_ptr++;
		remain--;
	}
	return (NULL);
}

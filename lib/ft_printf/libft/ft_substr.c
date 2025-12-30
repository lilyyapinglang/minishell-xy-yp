/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:29:20 by ylang             #+#    #+#             */
/*   Updated: 2025/05/14 23:38:30 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*writestr;
	char	*substr;
	size_t	len_s;
	size_t	substr_len;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (!*s || !len || start >= len_s)
		return (ft_strdup(""));
	if (len_s - start >= len)
		substr_len = len;
	else
		substr_len = len_s - start;
	substr = (char *)malloc((substr_len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	writestr = substr;
	while (substr_len)
	{
		*writestr++ = *(s + start++);
		substr_len--;
	}
	*writestr = '\0';
	return (substr);
}

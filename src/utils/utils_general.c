/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:16:29 by ylang             #+#    #+#             */
/*   Updated: 2026/02/13 21:16:59 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "utils.h"

// TODO
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
// TODO
// ssize_t	ft_write_fd(const char *s, int fd)
// {
// 	return (1);
// }

// TODO
int	check_all_digit(char *str)
{
	if (!str || *str == '\0')
		return (0);
	while (*str)
	{
		if (ft_isdigit(*str) != 1)
			return (0);
		str++;
	}
	return (1);
}

int	is_only_n(char *str)
{
	while (*str)
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

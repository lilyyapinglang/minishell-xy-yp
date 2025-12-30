/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:08:46 by ylang             #+#    #+#             */
/*   Updated: 2025/06/23 11:35:13 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_str_tolower(char *str)
{
	char	*ptr;

	ptr = str;
	while (*ptr)
	{
		*ptr = ft_tolower(*ptr);
		ptr++;
	}
	return (str);
}

int	handle_uint(unsigned int n, unsigned int base, int lowercase)
{
	char			*str;
	unsigned int	len;

	len = 0;
	str = ft_utoa_base(n, base);
	if (!str)
		return (-1);
	if (lowercase)
		ft_str_tolower(str);
	ft_putstr_fd(str, FILE_DESCRIPTOR);
	len = ft_strlen(str);
	free(str);
	return (len);
}

int	handle_ptr(unsigned long long addr)
{
	char	*str;
	int		len;
	char	*temp;

	if (addr == 0)
	{
		str = ft_strdup("(nil)");
		if (!str)
			return (-1);
	}
	else
	{
		temp = ft_ulltoa_base(addr, 16);
		if (!temp)
			return (-1);
		ft_str_tolower(temp);
		str = ft_strjoin("0x", temp);
		free(temp);
		if (!str)
			return (-1);
	}
	ft_putstr_fd(str, FILE_DESCRIPTOR);
	len = ft_strlen(str);
	free(str);
	return (len);
}

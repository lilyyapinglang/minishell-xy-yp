/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:28:00 by ylang             #+#    #+#             */
/*   Updated: 2025/06/23 11:29:37 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

unsigned long long	ft_ullonglen_base(unsigned long long n, int base)
{
	unsigned long long	len;

	len = 0;
	if (base < 2 || base > 16)
		return (0);
	if (!n)
		return (1);
	n /= base;
	while (n)
	{
		n /= base;
		len++;
	}
	return (len);
}

char	*ft_utoa_base(unsigned int n, unsigned int base)
{
	return (ft_ulltoa_base(n, base));
}

char	*ft_ulltoa_base(unsigned long long n, unsigned int base)
{
	unsigned long long	len;
	char				*str;
	char				tab[17];

	if (base > 16 || base < 2)
		return (NULL);
	if (!n)
		return (ft_strdup("0"));
	ft_strlcpy(tab, "0123456789ABCDEF", 17);
	len = ft_ullonglen_base(n, base) + 1;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = tab[n % base];
		n /= base;
	}
	return (str);
}

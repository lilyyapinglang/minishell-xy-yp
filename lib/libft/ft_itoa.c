/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xueyan_wang <xueyan_wang@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:50:03 by xuewang           #+#    #+#             */
/*   Updated: 2026/03/03 09:19:40 by xueyan_wang      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	len_cal(int n)
{
	size_t	sign;
	size_t	len;

	sign = 0;
	len = 0;
	if (n == -2147483648)
		return (11);
	if (n >= 0 && n <= 9)
		return (sign + 1);
	if (n < 0)
	{
		sign = 1;
		n = (-1) * n;
	}
	while (n >= 10)
	{
		n = n / 10;
		len++;
	}
	return (len + sign + 1);
}

char	*ft_itoa(int n)
{
	size_t	sign;
	size_t	len;
	char	*dest;
	long	nbr;

	sign = 0;
	nbr = n;
	len = len_cal(nbr);
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	if (nbr < 0)
	{
		dest[0] = '-';
		sign = 1;
		nbr = (-1) * nbr;
	}
	dest[len] = '\0';
	while (len > sign)
	{
		dest[len - 1] = nbr % 10 + '0';
		nbr = nbr / 10;
		len--;
	}
	return (dest);
}
/*
int	main(void)
{
	int	n;

	n = 9;
	printf("%zu\n",len_cal(n));
	return (0);
}

int	main(void)
{
	int	n;
	
	n = -2356;
	printf("%s\n",ft_itoa(n));
	return (0);
}
*/

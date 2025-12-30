/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:11:20 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/06 22:02:41 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//#include<stdio.h>

int	ft_atoi(const char *str)
{
	size_t	i;
	long	sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = (-1) * sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + str[i] - '0';
		i++;
	}
	return ((sign) * (nbr));
}
/*
int	main(int ac, char** av)
{
	ac = 2;
	printf("%d\n",ft_atoi(av[1]));
	printf("%d\n", atoi(av[1]));
	return (0);
}
*/

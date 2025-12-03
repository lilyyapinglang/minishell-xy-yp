/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:08:22 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 13:14:50 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include<limits.h>
//#include<stdlib.h>
//#include<unistd.h>
#include"libft.h"

void	ft_putchar(long n, int fd)
{
	char	c;

	c = n + '0';
	write (fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	nbr;

	nbr = n;
	if (nbr < 0)
	{
		write (fd, "-", 1);
		nbr = -nbr;
	}
	if (nbr >= 0 && nbr <= 9)
		ft_putchar(nbr, fd);
	if (nbr >= 10)
	{
		ft_putnbr_fd(nbr / 10, fd);
		ft_putnbr_fd(nbr % 10, fd);
	}
}
/*
//if (nbr == -2147483648)
        {
                write (fd, "-2147483648", 11);
        }
*/

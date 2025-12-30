/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:28:00 by ylang             #+#    #+#             */
/*   Updated: 2025/06/18 17:23:37 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long num)
{
	int	count;

	if (num == 0)
		return (1);
	count = 0;
	while (num)
	{
		num /= 10;
		count++;
	}
	return (count);
}

static char	*convert_to_string(long value, int digit_count, int is_negative)
{
	char	*str;
	int		i;

	str = malloc(digit_count + is_negative + 1);
	if (!str)
		return (NULL);
	str[digit_count + is_negative] = '\0';
	i = digit_count + is_negative - 1;
	while (i >= is_negative)
	{
		str[i] = (value % 10) + '0';
		value /= 10;
		i--;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	int		digit_count;
	long	absolute_value;
	int		is_negative;

	is_negative = 0;
	absolute_value = n;
	if (n < 0)
	{
		is_negative = 1;
		absolute_value = -absolute_value;
	}
	digit_count = count_digits(absolute_value);
	return (convert_to_string(absolute_value, digit_count, is_negative));
}

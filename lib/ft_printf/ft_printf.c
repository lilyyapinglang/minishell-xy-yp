/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:24:06 by ylang             #+#    #+#             */
/*   Updated: 2025/06/23 11:30:19 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

static int	handle_char(char c)
{
	ft_putchar_fd(c, FILE_DESCRIPTOR);
	return (1);
}

static int	handle_str(char *str)
{
	if (!str)
		str = "(null)";
	ft_putstr_fd(str, FILE_DESCRIPTOR);
	return (ft_strlen(str));
}

static int	handle_int(int n)
{
	char	*str;
	int		len;

	str = ft_itoa(n);
	if (!str)
		return (-1);
	ft_putstr_fd(str, FILE_DESCRIPTOR);
	len = ft_strlen(str);
	free(str);
	return (len);
}

static int	handle_format(char *char_after_perc, va_list args)
{
	if (*char_after_perc == 'c')
		return (handle_char(va_arg(args, int)));
	if (*char_after_perc == 's')
		return (handle_str(va_arg(args, char *)));
	if (*char_after_perc == 'd' || *char_after_perc == 'i')
		return (handle_int(va_arg(args, int)));
	if (*char_after_perc == '%')
		return (handle_char('%'));
	if (*char_after_perc == 'u')
		return (handle_uint(va_arg(args, unsigned int), 10, 0));
	if (*char_after_perc == 'X')
		return (handle_uint(va_arg(args, unsigned int), 16, 0));
	if (*char_after_perc == 'x')
		return (handle_uint(va_arg(args, unsigned int), 16, 1));
	if (*char_after_perc == 'p')
		return (handle_ptr((unsigned long long)va_arg(args, void *)));
	return (0);
}

int	ft_printf(const char *format_str, ...)
{
	char	*next_perc_char;
	char	*char_after_perc;
	char	*preceeding_str;
	int		char_count;
	va_list	args;

	char_count = 0;
	va_start(args, format_str);
	next_perc_char = ft_strchr(format_str, '%');
	while (next_perc_char)
	{
		preceeding_str = ft_substr(format_str, 0, next_perc_char - format_str);
		if (!preceeding_str)
			return (-1);
		ft_putstr_fd(preceeding_str, FILE_DESCRIPTOR);
		char_count = char_count + next_perc_char - format_str;
		free(preceeding_str);
		char_after_perc = next_perc_char + 1;
		char_count += handle_format(char_after_perc, args);
		format_str = char_after_perc + 1;
		next_perc_char = ft_strchr(format_str, '%');
	}
	char_count += handle_str((char *)format_str);
	va_end(args);
	return (char_count);
}

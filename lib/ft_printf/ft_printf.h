/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:30:09 by ylang             #+#    #+#             */
/*   Updated: 2025/06/23 11:28:29 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>

# define FILE_DESCRIPTOR 1

char	*ft_str_tolower(char *str);
int		handle_ptr(unsigned long long addr);
int		handle_uint(unsigned int n, unsigned int base, int lowercase);
int		ft_printf(const char *format_str, ...);
char	*ft_utoa_base(unsigned int n, unsigned int base);
char	*ft_ulltoa_base(unsigned long long n, unsigned int base);

#endif

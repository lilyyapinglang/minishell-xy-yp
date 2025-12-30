/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:47:17 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/06 22:41:23 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//#include<stdio.h>
//#include<string.h>
char	*ft_strdup(const char *str)
{
	int		i;
	int		len;
	char	*dest;

	len = ft_strlen(str);
	i = 0;
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*
int	main(int ac, char ** av)
{
	ac = 2;
	printf("%s\n", ft_strdup(av[1]));
	printf("%s\n", strdup(av[1]));
	return (0);
}
*/

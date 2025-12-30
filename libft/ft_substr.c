/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:33:46 by xuewang           #+#    #+#             */
/*   Updated: 2025/05/12 19:32:45 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
#include<stdlib.h>
#include<stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*dest;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	dest = malloc (sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while ((i < len))
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*if (start >= str_len)
                return (NULL);
int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage: ./test_substr \"<string>\" <start> <len>\n");
        return 1;
    }

    const char *input = argv[1];
    unsigned int start = (unsigned int)atoi(argv[2]);
    size_t len = (size_t)atoi(argv[3]);

    char *result = ft_substr(input, start, len);

    if (result)
    {
        printf("Input     : \"%s\"\n", input);
        printf("Start     : %u\n", start);
        printf("Length    : %zu\n", len);
        printf("Substring : \"%s\"\n", result);
        free(result);
    }
    else
    {
        printf("ft_substr returned NULL\n");
    }

    return 0;
}
*/

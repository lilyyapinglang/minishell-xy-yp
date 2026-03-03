/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:23:21 by ylang             #+#    #+#             */
/*   Updated: 2026/03/03 16:42:53 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*find_nl_getline(char *nlchar_pos, char **currentstr_addr)
{
	size_t	line_len;
	char	*line;
	char	*temp;

	line_len = nlchar_pos - *currentstr_addr + 1;
	line = ft_substr(*currentstr_addr, 0, line_len);
	temp = *currentstr_addr;
	*currentstr_addr = ft_strdup(nlchar_pos + 1);
	free(temp);
	return (line);
}

int	read_until_nl(int fd, char **nlchar_pos_addr, char **currentstr_addr,
	char **buffer_addr)
{
	char	*temp;
	int		bytesread;

	while (*nlchar_pos_addr == NULL)
	{
		bytesread = read(fd, *buffer_addr, BUFFER_SIZE);
		if (bytesread == 0)
			break ;
		if (bytesread < 0)
		{
			free(*buffer_addr);
			eof_no_nl_empty_or_readerror(currentstr_addr);
			return (0);
		}
		(*buffer_addr)[bytesread] = '\0';
		temp = *currentstr_addr;
		*currentstr_addr = ft_strjoin(*currentstr_addr, *buffer_addr);
		free(temp);
		*nlchar_pos_addr = ft_strchr(*currentstr_addr, '\n');
	}
	return (1);
}

static char	**gnl_currentstr_ref(void)
{
	static char	*currentstr;

	return (&currentstr);
}

char	*get_next_line(int fd)
{
	char	**currentstr;
	char	*buffer;
	char	*nlchar_pos;

	currentstr = gnl_currentstr_ref();
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (*currentstr == NULL)
		*currentstr = ft_strdup("");
	nlchar_pos = ft_strchr(*currentstr, '\n');
	if (read_until_nl(fd, &nlchar_pos, currentstr, &buffer) == 0)
		return (NULL);
	free(buffer);
	if (nlchar_pos)
		return (find_nl_getline(nlchar_pos, currentstr));
	else if (*currentstr && **currentstr == '\0')
		return (eof_no_nl_empty_or_readerror(currentstr));
	else
		return (eof_no_nl_notempty(currentstr));
}

void	gnl_clear_static_buffer(void)
{
	char	**currentstr;

	currentstr = gnl_currentstr_ref();
	if (*currentstr)
	{
		free(*currentstr);
		*currentstr = NULL;
	}
}

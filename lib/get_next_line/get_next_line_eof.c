/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_eof.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylang <ylang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:16:32 by ylang             #+#    #+#             */
/*   Updated: 2026/03/03 16:40:40 by ylang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*eof_no_nl_empty_or_readerror(char **currentstr_addr)
{
	if (*currentstr_addr)
		free(*currentstr_addr);
	*currentstr_addr = NULL;
	return (NULL);
}

char	*eof_no_nl_notempty(char **currentstr_addr)
{
	char	*line;

	line = ft_strdup(*currentstr_addr);
	free(*currentstr_addr);
	*currentstr_addr = NULL;
	return (line);
}

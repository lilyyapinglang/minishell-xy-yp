/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:23:31 by ylang             #+#    #+#             */
/*   Updated: 2026/03/03 17:50:13 by lilypad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif

size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *str, int searchedChar);
char	*ft_strjoin(char const *s1, char const *s2);
char	*eof_no_nl_empty_or_readerror(char **currentstr_addr);
char	*eof_no_nl_notempty(char **currentstr_addr);
char	*get_next_line(int fd);
void	gnl_clear_static_buffer(void);

#endif

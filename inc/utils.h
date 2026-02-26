/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:51 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/26 20:08:52 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef UTILS_H
# define UTILS_H
// utils-general

int		ft_strcmp(const char *s1, const char *s2);
ssize_t	ft_write_fd(const char *s, int fd);
int		check_all_digit(char *str);
int		is_only_n(char *str);
int		count_strs(char **argv);

#endif
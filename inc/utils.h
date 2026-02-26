/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuewang <xuewang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:12:50 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/25 20:14:12 by xuewang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
// utils-general

int		ft_strcmp(const char *s1, const char *s2);
ssize_t	ft_write_fd(const char *s, int fd);
int		check_all_digit(char *str);
int		is_only_n(char *str);

#endif
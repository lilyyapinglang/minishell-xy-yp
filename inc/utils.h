/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilypad <lilypad@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 20:08:51 by xuewang           #+#    #+#             */
/*   Updated: 2026/02/27 17:36:39 by lilypad          ###   ########.fr       */
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
void	free_strs(char **envp);

#endif
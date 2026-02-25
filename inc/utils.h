
#ifndef UTILS_H
# define UTILS_H
// utils-general

int		ft_strcmp(const char *s1, const char *s2);
ssize_t	ft_write_fd(const char *s, int fd);
int		check_all_digit(char *str);
int		is_only_n(char *str);
int		count_strs(char **argv);

#endif
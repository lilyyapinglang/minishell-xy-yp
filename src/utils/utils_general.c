
// TODO
int		ft_strcmp(const char *s1, const char *s2);
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
// TODO
ssize_t	ft_write_fd(const char *s, int fd);
{
	return (1);
}

// TODO
int	is_only_n(char **strs)
{
	return (1);
}
// TODO
int	check_all_digit(char *str)
{
	return (1);
}
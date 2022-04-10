#include "includes/minishell.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putendl_fd(char *str, int fd)
{
	char	nline;

	nline = '\n';
	write(fd, str, ft_strlen(str));
	write(fd, &nline, 1);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

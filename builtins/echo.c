//#include "minishell.c"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((unsigned char)s1[i] == (unsigned char)s2[i]
		&& s1[i] != '\0' && s2[i] != '\0' && i < n - 1)
		i++;
	if (n > 0)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	else
		return (0);
}

static void	ft_print_args(int32_t argc, char **argv, bool newline)
{
	int32_t	i;

	i = 1;
	if (!newline)
		i++;
	while (argc >= 2 && i <= argc - 2)
	{
		ft_putstr(argv[i++]);
		ft_putchar(' ');
	}
	ft_putstr(argv[i]);
	if (newline)
		ft_putchar('\n');
}

int32_t	ft_echo(int argc, char **argv)
{
	bool	newline;

	newline = true;
	if (argc == 1)
	{
		ft_putchar('\n');
		return (EXIT_SUCCESS);
	}
	if (argc >= 2)
	{
		if (ft_strncmp(argv[1], "-n", 2) == 0)
			newline = false;
		if (argc < 3)
			ft_putchar('\0');
	}
	ft_print_args(argc, argv, newline);
	return (EXIT_SUCCESS);
}

#include <stdlib.h>
#include <unistd.h>

int	ft_atoi(const char *str)
{
	int	i;
	int	rslt;
	int	neg;

	i = 0;
	rslt = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		rslt = rslt * 10 + (str[i] - 48);
		i++;
	}
	return (rslt * neg);
}

size_t	ft_strlen(const char *s)
{
	if (*s == '\0')
		return (0);
	return (ft_strlen(s + 1) + 1);
}

void	ft_putendl_fd(char *s, int fd)
{
	char	nline;

	nline = '\n';
	write(fd, s, ft_strlen(s));
	write(fd, &nline, 1);
}

int32_t	ft_exit(int argc, char **argv)
{
	(void)argc;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!argv[1])
		exit(0);
	exit(ft_atoi(argv[1]));
}

int	main(int argc, char **argv)
{
	ft_exit(argc, argv);
}

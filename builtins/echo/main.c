#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	if (*s == '\0')
		return (0);
	return (ft_strlen(s + 1) + 1);
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

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	i = 2;
	while (argv[i])
		i++;
	if (argc < 2)
		return (0);
	else
	{
		if (ft_strncmp(argv[1], "echo", ft_strlen(argv[1])))
			return (0);
		else
		{
			if (!ft_strncmp(argv[2], "-n", ft_strlen(argv[2])))
			{
				j = 3;
				i--;
				while (i > j)
				{
					printf("%s ", argv[j]);
					j++;
				}
				printf("%s", argv[j]);
			}
			else
			{
				j = 2;
				i--;
				while (i > j)
				{
					printf("%s ", argv[j]);
					j++;
				}
				printf("%s\n", argv[j]);
			}
		}
	}
}

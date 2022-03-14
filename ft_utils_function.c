#include "includes/minishell.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_calloc(size_t quantity, size_t type_size)
{
	void	*result;

	result = malloc(quantity * type_size);
	if (result == NULL)
		return (NULL);
	ft_bzero(result, quantity * type_size);
	return (result);
}

char	*ft_strndup(const char *str, size_t n, t_bool free_str)
{
	size_t	index;
	char	*result;

	if (!str)
		return (NULL);
	result = ft_calloc(n + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	index = 0;
	while (str[index] && index < n)
	{
		result[index] = str[index];
		index++;
	}
	while (index <= n)
	{
		result[index] = 0;
		index++;
	}
	if (free_str)
		free((char *)str);
	return (result);
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

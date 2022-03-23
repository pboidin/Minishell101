#include "../../includes/minishell.h"

char	*ft_strcpy(char *dst, const char *src)
{
	char	*save;

	save = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (save);
}

char	*ft_strdup(const char *str)
{
	char	*cpy;
	size_t	len;

	len = ft_strlen(str);
	cpy = malloc(len + 1);
	if (!cpy)
		return (NULL);
	return (ft_strcpy(cpy, str));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		str = (char *)malloc(sizeof(char));
		if (!str)
			return (NULL);
		str[0] = 0;
		return (str);
	}
	i = 0;
	s += start;
	while (s[i] && i < len)
		i++;
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (++i < len && s[i])
		str[i] = s[i];
	str[i] = 0;
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*join;

	i = 0;
	j = 0;
	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s1 || !s2 || !join)
		return (0);
	while (s1[i])
	{
		join[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		join[j] = s2[i];
		j++;
		i++;
	}
	join[j] = '\0';
	return (join);
}

void	*ft_memset(void *target, int char_to_set, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	str = target;
	while (i < n)
	{
		str[i] = (unsigned char)char_to_set;
		i++;
	}
	return (target);
}

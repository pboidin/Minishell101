#include "../../includes/minishell.h"
#include <dirent.h>
#include <string.h>

/*int    test(char *s1, char *s2)
{
    if (!*s1 && !*s2)
        return (1);
    else if (*s1 == *s2 && *s1 != '*')
        return (match(s1 + 1, s2 + 1));
    else if (*s1 == '*' && *s2 == '*')
        return (match(s1 + 1, s2));
    else if (*s2 == '*' && !*s1)
        return (match(s1, s2 + 1));
    else if (*s2 == '*' && *s2 && *s1)
        return (match(s1, s2 + 1) || match(s1 + 1, s2));
    else
        return (0);
}*/

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strcpy1(char *dst, const char *src)
{
	char	*save;

	save = dst;
	while (src && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (save);
}

char	*ft_strdup1(const char *str)
{
	char	*cpy;
	size_t	len;

	len = ft_strlen(str);
	cpy = malloc(len + 1);
	if (!cpy)
		return (NULL);
	return (ft_strcpy1(cpy, str));
}

int	ft_iffirst(char *str, char c)
{
	if (str[0] == c)
		return (1);
	return (0);
}

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_has_wildcards(char *str)
{
	if (ft_strchr(str, '\'') == 1 || ft_strchr(str, '\"') == 1)
		return (0);
	if (ft_strchr(str, '*') && !ft_strchr(str, '/'))
		return (1);
	return (0);
}

/*int	ft_wildcards(char **tab, char *arg)
{
	struct dirent	*de;
  	DIR				*dr;
    int				i;
	int				j;
	(void)arg;
	// char			*pwd;
	// char			**bef;

	// pwd = ft_genv("PWD", info->env);
	dr = opendir(".");
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return (0);
    }
	i = 0;
	while ((de = readdir(dr)) != NULL)
		i++;
	closedir(dr);

	dr = opendir(".");
	j = 0;
	while ((de = readdir(dr)) != NULL)
	{
		tab = (char **)malloc(sizeof(char *) * (i + 1));
		if (!tab)
			return (0);
		if (ft_iffirst(de->d_name, '.') == 0)
		{
			tab[j]= ft_strdup1(de->d_name);
			printf("tab[%d] = %s\n\n", j, tab[j]);
			j++;
		}
	}
	closedir(dr);
	free(tab);
    return (1);
}*/

int main (int argc, char **argv)
{
	char	**tab;

	tab = NULL;
	if (argc != 2)
		return (0);
	if (ft_has_wildcards(argv[1]) != 0)
	{
		ft_wildcards(tab, argv[1]);
		ft_wild();
		return (1);
	}
	return (0);
}


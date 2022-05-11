# include "minishell.h"

char *ft_strcat(char *dest, char *src)
{
	int i;
	int j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void	ft_wild_two(t_block *src, char ***tab, t_wild *list)
{
	int     i;

	i = 0;
	while (list)
	{
        if (ft_compare(list->path, src, 0, 0) == 1)
        {
        	(*tab)[i] = ft_strcpy((*tab)[i], list->path);
        	i++;
        }
		list = list->next;
	}
    (*tab)[i] = NULL;
}

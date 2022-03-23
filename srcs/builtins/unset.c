#include "../includes/minishell.h"

static void	ft_delete_join(t_env *aux, int c)
{
	t_env	*tmp;

	tmp = NULL;
	if (c == 0)
	{
		aux = g_info.env;
		g_info.env = (g_info.env)->next;
		ft_lstdelone(aux, free);
		return ;
	}
	if (aux->next && aux->next->next)
		tmp = aux->next->next;
	ft_lstdelone(aux->next, free);
	aux->next = tmp;
}

static t_env	*ft_build_str(const char *unset, char **tmp)
{
	char	*tmp1;
	t_env	*aux;

	ft_strlcpy(*tmp, unset, ft_strlen(unset) + 1);
	tmp1 = *tmp;
	*tmp = ft_strjoin(tmp1, "=");
	free(tmp1);
	aux = g_info.env;
	return (aux);
}

static t_env	*ft_find_pos(const char *unset, int *c, char *tmp)
{
	t_env	*aux;

	if (unset == NULL)
		return (NULL);
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(unset) + 1));
	if (!tmp)
		return (NULL);
	aux = ft_build_str(unset, &tmp);
	if (ft_strncmp((char *)aux->next->value, tmp, ft_strlen(tmp)) == 0)
	{
		free (tmp);
		return (aux);
	}
	while (aux && aux->next)
	{
		(*c)++;
		if (ft_strncmp((char *)aux->next->value, tmp, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			return (aux);
		}
		aux = aux->next;
	}
	free(tmp);
	return (NULL);
}

void	ft_unset_exec(char *const *unset, t_env *aux, char *tmp, int i)
{
	int	j;

	j = 0;
	aux = ft_find_pos(unset[i], &i, tmp);
	if (aux != NULL)
		ft_delete_join(aux, j);
}

int	ft_unset(char **unset)
{
	char	*tmp;
	int		i;
	int		ret;
	t_env	*aux;

	aux = g_info.env;
	i = 0;
	tmp = NULL;
	ret = 0;
	if (unset[1] && !aux)
		return (0);
	else if (unset[1] && aux)
	{
		while (unset[++i])
		{
			if (ft_unset_handle(unset[i]) == 0)
				ft_unset_exec(unset, aux, tmp, i);
			else
				ret = 1;
		}
	}
	return (ret);
}
#include "../8_Minishell/Minishell101/includes/minishell.h"

static char	*ft_set_env(char *pwd, const char *lvl_sh, char *aux)
{
	/*not finish yet*/
}

static int	ft_sh_safe(t_env *aux, char **tmp, int *a)
{
	char	*tmp2;

		if ((*a) < 0)
		{
			(*a) = 1;
			free((*tmp));
			(*tmp) = ft_itoa((*a));
			tmp2 = ft_strjoin("SHLVL=", (*tmp));
			free(aux->value);
			aux->value = ft_strdup(tmp2);
			return (1);
		}
		return (0);
}

static void	ft_sh_incre(t_env *aux, char *tmp, int a)
{
	char	*tmp2;

	a++;
	free(tmp);
	tmp = ft_itoa(aux);
	tmp2 = ft_strjoin("SHLVL=", tmp);
	free(aux->value);
	aux->value = ft_strdup(tmp2);
	free(tmp2);
	free(tmp);
}

static void ft_sh_upd(void)
{
	t_env *aux;
	char	*tmp;
	int		a;

	aux = g_info.env;
	while(aux)
	{
		if (ft_strncmp("SHLVL=", (char *)aux->value, 6) == 0)
			break;
		aux = aux->next;
	}
	if (!aux)
	{
		ft_lstadd_back(g_info.env, ft_lstnew(ft_strdup("SHLVL=1")));
		return;
	}
	tmp = ft_substr((char *)aux->value, 6, ft_strlen((char *)aux->value) - 6);
	a = ft_atoi(tmp);
	if (ft_sh_safe(aux, &tmp, &a) == 1)
		return ;
	else
		ft_sh_incre(aux, tmp, a);
}

/* initialise environnement */
void	ft_env_fill(char **env)
{
	int		i;
	char	*pwd;
	char	*lvl_sh;
	char	*aux;

	i = -1;
	g_info.env = (t_env **)ft_calloc(1, (sizeof(t_env *)));
	pwd = malloc(sizeof(char) * PATH_MAX);
	if (!pwd || !g_info.env)
		return;
	aux = NULL;
	lvl_sh = ft_strdup("SHLVL=1");
	if (env[0])
	{
		while (env[++i])
			ft_lstadd_back(g_info.env, ft_lstnew((void *) ft_strdup(env[i])));
		ft_sh_upd();
	}
	else
		aux = 
}
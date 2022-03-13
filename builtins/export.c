#include "minishell.h"

static char	**ft_lst_to_ptr(void)
{
	char	**env;
	int		i;
	t_list	*env2;

	i = 0;
	env = malloc(sizeof(char *) * (ft_lstsize(*g_info.env) + 1));
	if (!env)
		return (NULL);
	env2 = *g_info.env;
	while (env2)
	{
		env[i++] = ft_strdup(env2->data);
		env2 = evn2->next;
	}
	env[i] = NULL;
	return (env);
}

static void	ft_swap_str(int i, int j, char **env, char *tmp)
{
	if (ft_lenvar(env[j]) > ft_lenvar(env[i]))
	{
		if (ft_strncmp(env[j], env[i], ft_lenvar(env[j])) > 0)
		{
			tmp = ft_strdup(env[j]);
			free(env[j]);
			env[j] = ft_strdup(env[i]);
			free(env[i]);
			env[i] = tmp;
		}
	}
	else
	{
		if (ft_strncmp(env[j], env[i], ft_lenvar(env[i])) > 0)
		{
			tmp = ft_strdup(env[j]);
			free(env[j]);
			env[j] = ft_strdup(env[i]);
			free(env[i]);
			env[i] = tmp;
		}
	}
}

static void	ft_lst_printer(char **env)
{
	int	i;
	int	j;

	i = -1;
	while (env[++i])
	{
		j = -1;
		while (env[i][++j] != '\0')
		{
			write(STDOUT_FILENO, &env[i][j], 1);
			if (env[i][j] == '=')
				break;
		}
		write(STDOUT_FILENO, "\"", 1);
		while (env[i][++j] != '\0')
			write(STDOUT_FILENO, &env[i][j], 1);
		if (env[i][j] == '\0')
			write(STDOUT_FILENO, "\"", 1);
		write(1, "\n", 1);
		free(env[i]);
	}
	free(env);
}

static void	ft_lst_sort(void)
{
	char	*tmp;
	char	**env;
	int		i;
	int		j;

	tmp = NULL;
	i = -1;
	env = ft_lst_to_str();
	while (env[++i])
	{
		j = -1;
		while (env[++j])
		ft_swap_str(i, j, env, tmp);
	}
	ft_lst_printer(env);
}

int	ft_export(char **new_env)
{
	char	*env;
	char	*env2;
	char 	*tmp;

	env = NULL;
	env2 = NULL;
	tmp = NULL;
	if (!new_env)
		ft_lst_sort();
	else
		ft_export_var(new_env, tmp, env, env2);	
	return (0)
}

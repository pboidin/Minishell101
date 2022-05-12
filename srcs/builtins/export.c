/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:21 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:23 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_lst_to_str(t_info *info)
{
	char	**env;
	int		i;
	t_env	*env2;

	i = 0;
	env = malloc(sizeof(char *) * (ft_lstsize(info->env) + 1));
	if (!env)
		return (NULL);
	env2 = info->env;
	while (env2)
	{
		env[i++] = ft_strdup(env2->value);
		env2 = env2->next;
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
				break ;
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

static void	ft_lst_sort(t_info *info)
{
	char	*tmp;
	char	**env;
	int		i;
	int		j;

	tmp = NULL;
	i = -1;
	env = ft_lst_to_str(info);
	while (env[++i])
	{
		j = -1;
		while (env[++j])
			ft_swap_str(i, j, env, tmp);
	}
	ft_lst_printer(env);
}

int	ft_export(char **new_env, t_info *info)
{
	if (!new_env)
		ft_lst_sort(info);
	else
		ft_export_var(new_env, info);
	return (0);
}

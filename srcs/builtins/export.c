/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:21 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/13 16:27:40 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		write(STDOUT_FILENO, "declare -x ", 11);
		j = -1;
		while (env[i][++j] != '\0')
		{
			write(STDOUT_FILENO, &env[i][j], 1);
			if (env[i][j] == '=')
				break ;
		}
		if (env[i][j] != '\0')
		{
			write(STDOUT_FILENO, "\"", 1);
			write(STDOUT_FILENO, &env[i][j] + 1, ft_strlen(&env[i][j] + 1));
			write(STDOUT_FILENO, "\"", 1);
		}
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
	env = join_env(info);
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
	if (!new_env[1])
		ft_lst_sort(info);
	else
		ft_export_var(new_env, info);
	return (0);
}

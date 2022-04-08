/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:01 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:02 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print(t_info *info)
{
	char	*values;
	char	*t_name;
	int		i;
	t_env	*env;

	env = info->env;
	while (env)
	{
		i = 0;
		values = (char *)env->value;
		t_name = (char *)env->name; 
		if (values[i] && t_name[i])
		{
			write(1, env->name, ft_strlen((char *)env->name));
			write(1, "=", 1);
			write(1, env->value, ft_strlen((char *)env->value));
			write(1, "\n", 1);
		}
		env = env->next;
		i++;
	}
}

int	ft_env(char **tab, t_info *info)
{
	if (!info->env)
		return (0);
	if (tab[1] == NULL)
		ft_print(info);
	else
	{
		write(STDERR_FILENO, "env: ", 5);
		write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	}
	return (0);
}

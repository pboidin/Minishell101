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

static void	ft_print(t_info *info, int fd)
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
			write(fd, env->name, ft_strlen((char *)env->name));
			write(fd, "=", 1);
			write(fd, env->value, ft_strlen((char *)env->value));
			write(fd, "\n", 1);
		}
		env = env->next;
	}
}

int	ft_env(char **tab, t_info *info, t_cmd *cmd)
{
	int	fd;

	fd = 1;
	if (cmd->out)
		fd = cmd->out->fd;
	if (!info->env)
		return (0);
	if (tab[1] == NULL)
		ft_print(info, fd);
	else
	{
		write(STDERR_FILENO, "env: ", 5);
		write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		return (127);
	}
	return (0);
}

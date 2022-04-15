/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:46:52 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:46:54 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_putstr_nnl(char *const *tab, int i, int fd)
{
	while (tab[i])
	{
		write(fd, tab[i], ft_strlen(tab[i]));
		if (tab[i + 1])
			write(fd, " ", 1);
		i++;
	}
}

static void	ft_putstr_nl(char *const *tab, int fd)
{
	int	i;

	i = 1;
	while (tab[i])
	{
		write(fd, tab[i], ft_strlen(tab[i]));
		if (tab[i + 1])
			write(fd, " ", 1);
		i++;
	}
	write(fd, "\n", 1);
}

static int	ft_check_flag(char *tab)
{
	int	i;

	if (tab && ft_strncmp(tab, "-n", 2) == 0)
	{
		i = 2;
		while (tab[i])
		{
			if (tab[i] != 'n')
				return (1);
			i++;
		}
	}
	else
		return (1);
	return (0);
}

int	ft_echo(char **tab, t_cmd *cmd)
{
	int	i;
	int	fd;

	fd = 1;
	i = 1;
	if (cmd->out)
		fd = cmd->out->fd;
	if (!tab[i])
	{	
		write(fd, "\n", 1);
		return (0);
	}
	if (ft_check_flag(tab[i]) == 0)
	{
		i++;
		while (tab[i] && ft_check_flag(tab[i]) == 0)
			i++;
		if (!tab[i])
			return (0);
		ft_putstr_nnl(tab, i, fd);
	}
	else
		ft_putstr_nl(tab, fd);
	return (0);
}

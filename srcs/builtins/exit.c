/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:12 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:14 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_exit_code(char *const *tab, int i, t_info *info)
{
	if (tab && tab[0] && tab[1] && tab[2])
	{
		if (info->is_child == FALSE)
			write(STDERR_FILENO, "exit\n", 5);
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		return (1);
	}
	while (tab[1] && tab[1][i])
	{
		if (!ft_isdigit(tab[1][i++]))
		{
			if (info->is_child == FALSE)
				write(STDERR_FILENO, "exit\n", 5);
			write(STDERR_FILENO, "exit: ", 6);
			write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			info->status = 2;
			free_info(info);
			exit(2);
		}
	}
	return (0);
}

int	ft_exit(char **tab, t_info *info)
{
	int	exit_code;
	int	i;

	i = 0;
	if (tab[1] && (tab[1][i] == '+' || tab[1][i] == '-'))
		i++;
	if (ft_exit_code(tab, i, info) == 1)
		return (1);
	if (tab[1])
	{
		exit_code = ft_atoi(tab[1]);
		info->status = (unsigned char)exit_code;
		free_info(info);
		if (info->is_child == FALSE)
			write(STDERR_FILENO, "exit\n", 5);
		exit((unsigned char)exit_code);
	}
	free_info(info);
	if (info->is_child == FALSE)
		write(STDERR_FILENO, "exit\n", 5);
	exit(info->status);
	return (0);
}

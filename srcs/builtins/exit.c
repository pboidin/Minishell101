/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:12 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/02 16:04:42 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	lcl_ft_ulllen(unsigned long long n)
{
	size_t	nb_len;

	nb_len = 0;
	while (n >= 10)
	{
		nb_len++;
		n /= 10;
	}
	return (nb_len + 1);
}

static void	numeric_error(char *const *tab, t_info *info )
{
	write(STDERR_FILENO, "exit\n", 5);
	write(STDERR_FILENO, "exit: ", 6);
	write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	info->status = 2;
	free_info(info);
	exit(2);
}

static int	ft_exit_code(char *const *tab, int i, t_info *info)
{
	if (tab[1] && (tab[1][0] == '+' || tab[1][0] == '-'))
	{
		if (ft_strlen(tab[1]) < 2
			|| ft_strlen(tab[1]) > lcl_ft_ulllen(ULLONG_MAX))
			numeric_error(tab, info);
	}
	else if (tab[1] && ft_strlen(tab[1]) >= lcl_ft_ulllen(ULLONG_MAX))
		numeric_error(tab, info);
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
			numeric_error(tab, info);
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
	return (exit(info->status), 0);
}

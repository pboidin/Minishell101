/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_str_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:29:55 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/11 22:54:42 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	return ;
}

void	skip_whitespaces(char *cmd, int *i)
{
	if (!cmd)
		return ;
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 11:55:26 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/11 12:00:56 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_nb(char *nb)
{
	nb[0] = '0';
	nb[1] = '0';
	nb[2] = '0';
	nb[3] = '0';
	nb[4] = '0';
	nb[5] = '0';
	nb[6] = '0';
	nb[7] = '0';
	nb[8] = '0';
	nb[9] = '1';
	nb[10] = '\0';
}

static void	increase_nb(char *nb, int index)
{
	nb[index] += 1;
	if (nb[index] > '9')
	{
		if (index == 0)
			return ;
		nb[index] = '0';
		increase_nb(nb, (index - 1));
	}
}


char	*create_tmp(void)
{
	char	*path;
	char	nb[11];

	init_nb(nb);
	path = ft_strjoin(TMP_PATH, nb);
	if (!path)
		return (perror("Malloc error"), NULL);
	while (!access(path, F_OK))
	{
		free(path);
		increase_nb(nb, 9);
		if (nb[0] > '9')
			return (write(2, "Error creating here-document\n", 29), NULL);
		path = ft_strjoin(TMP_PATH, nb);
		if (!path)
			return (perror("Malloc error"), NULL);
	}
	return (path);
}

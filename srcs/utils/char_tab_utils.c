/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_tab_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:09:05 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 11:29:14 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

size_t	char_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**create_char_tab(size_t size)
{
	char	**tab;

	tab = (char **)ft_calloc((size + 1), sizeof(char *));
	if (!tab)
		return (perror("Malloc error"), NULL);
	return (tab);
}

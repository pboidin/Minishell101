/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_block_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 12:15:41 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 11:13:03 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_t_block(t_block *block)
{
	size_t	i;

	i = 0;
	while (block[i].str)
	{
		free(block[i].str);
		i++;
	}
	free(block);
}

void	free_t_block_tab(t_block **block_tab)
{
	size_t	i;

	i = 0;
	while (block_tab[i])
	{
		free_t_block(block_tab[i]);
		i++;
	}
	free(block_tab);
}

size_t	t_block_tab_size(t_block **tab)
{
	size_t	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	*t_block_to_str(t_block *block)
{
	size_t	i;
	char	*ret;
	char	*tmp;

	if (!block || !block[0].str)
		return (NULL);
	ret = ft_strdup(block[0].str);
	if (!ret)
		return (perror("Malloc error"), NULL);
	i = 1;
	while (block[i].str)
	{
		tmp = ft_strjoin(ret, block[i].str);
		if (!tmp)
			return (perror("Malloc error"), free(ret), NULL);
		free(ret);
		ret = tmp;
		i++;
	}
	return (ret);
}

char	**t_block_tab_to_char_tab(t_block **tab)
{
	size_t	tab_size;
	char	**new_args;

	if (!tab)
		return (NULL);
	tab_size = t_block_tab_size(tab);
	new_args = (char **)ft_calloc((tab_size + 1), sizeof(char *));
	if (!new_args)
		return (NULL);
	tab_size = 0;
	while (tab[tab_size])
	{
		new_args[tab_size] = t_block_to_str(tab[tab_size]);
		if (!new_args[tab_size])
			return (free_char_tab(new_args), NULL);
		tab_size++;
	}
	return (new_args);
}

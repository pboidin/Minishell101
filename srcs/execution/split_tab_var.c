/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tab_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:36:14 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 15:34:50 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block	*insert_back_t_block(t_block *line, size_t i, char *var)
{
	t_block	*new_line;
	size_t	j;

	new_line = (t_block *)ft_calloc((i + 2), sizeof(t_block));
	if (!new_line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_line[j] = line[j];
		j++;
	}
	new_line[j].str = var;
	return (new_line);
}

t_block	*insert_front_t_block(t_block *line, size_t i, char *var)
{
	t_block	*new_line;
	size_t	j;
	size_t	len;

	j = i + 1;
	len = 0;
	while (line[j].str)
	{
		j++;
		len++;
	}
	new_line = (t_block *)ft_calloc((len + 2), sizeof(t_block));
	if (!new_line)
		return (NULL);
	new_line[0].str = var;
	j = 1;
	len = i + 1;
	while (line[len].str)
	{
		new_line[j] = line[len];
		j++;
		len++;
	}
	return (new_line);
}

void	free_new_tab(t_block **new_tab, size_t x, int y)
{
	while (x > 0)
	{
		free(new_tab[y + x]);
		x--;
	}
	free(new_tab[y]);
	free(new_tab);
}

size_t	ins_vars(t_block **new_words_tab, int y, char **var, size_t nb_var)
{
	size_t	x;

	x = 1;
	while ((x + 1) < nb_var)
	{
		new_words_tab[y + x] = (t_block *)ft_calloc(2, sizeof(t_block));
		if (!new_words_tab[y + x])
			return (free_new_tab(new_words_tab, x, y), free_char_tab(var), 0);
		new_words_tab[y + x][0].str = var[x];
		x++;
	}
	return (x);
}

size_t	split_tab_var(t_block ***words_tab, size_t j, size_t i, char **var)
{
	size_t	x;
	int		y;
	size_t	nb_var;
	t_block	**new_words_tab;
	t_block	**f;

	nb_var = char_tab_size(var);
	new_words_tab = (t_block **)ft_calloc((j + nb_var + 1), sizeof(t_block *));
	if (!new_words_tab)
		return (free_char_tab(var), 0);
	y = -1;
	while (++y < (int)j)
		new_words_tab[y] = words_tab[0][y];
	new_words_tab[y] = insert_back_t_block(words_tab[0][y], i, var[0]);
	if (!new_words_tab[y])
		return (free(new_words_tab), free_char_tab(var), 0);
	x = ins_vars(new_words_tab, y, var, nb_var);
	if (!x)
		return (0);
	new_words_tab[y + x] = insert_front_t_block(words_tab[0][y], i, var[x]);
	if (!new_words_tab[y + x])
		return (free_new_tab(new_words_tab, x, y), free_char_tab(var), 0);
	f = *words_tab;
	*words_tab = new_words_tab;
	return (free(var), free(f[j][i].str), free(f[j]), free(f), nb_var);
}

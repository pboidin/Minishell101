/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_mask1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 17:45:24 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 19:03:25 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block	**init_block(void)
{
	t_block	**new_block;

	new_block = ft_calloc(2, sizeof(t_block *));
	if (!new_block)
	{
		g_signal = 1;
		return (perror("Malloc error"), NULL);
	}
	new_block[0] = ft_calloc(2, sizeof(t_block));
	if (!new_block[0])
	{
		g_signal = 1;
		return (free(new_block), perror("Malloc error"), NULL);
	}
	return (new_block);
}

t_block	**root_wild(void)
{
	t_block	**new_block;

	new_block = init_block();
	if (!new_block)
		return (NULL);
	new_block[0][0].str = ft_calloc(2, sizeof(char));
	if (!new_block[0][0].str)
	{
		g_signal = 1;
		return (free_t_block_tab(new_block), perror("Malloc error"), NULL);
	}
	new_block[0][0].str[0] = '/';
	return (new_block);
}

t_block	**same_block(t_block *block, int s[2], int e[2], char wild)
{
	char	tmp;
	t_block	**new_block;

	if (e[0] == 0 && e[1] == 0)
		return (root_wild());
	new_block = init_block();
	if (!new_block)
		return (NULL);
	tmp = block[e[0]].str[e[1]];
	block[e[0]].str[e[1]] = '\0';
	new_block[0][0].str = ft_strdup(&block[s[0]].str[s[1]]);
	block[e[0]].str[e[1]] = tmp;
	if (!new_block[0][0].str)
		return (free_t_block_tab(new_block), NULL);
	if (wild)
		new_block[0] = split_on_wild(new_block, block, s);
	return (new_block);
}

int	save_last_line(t_block *block, int e[2], int c[2], t_block **new_block)
{
	char	tmp;

	tmp = block[e[0]].str[e[1]];
	block[e[0]].str[e[1]] = '\0';
	new_block[0][0].str = ft_strcat_mal(new_block[0][0].str, block[c[0]].str);
	if (!new_block[0][0].str)
		return (free_t_block_tab(new_block), 1);
	block[e[0]].str[e[1]] = tmp;
	return (0);
}

t_block	**add_to_mask(int s[2], int e[2], char wild, t_block *block)
{
	t_block	**new_b;
	int		c[2];

	if ((e[0] == 0 && e[1] == 0) || s[0] == e[0])
		return (same_block(block, s, e, wild));
	new_b = init_block();
	if (!new_b)
		return (NULL);
	new_b[0][0].str = ft_strdup(&block[s[0]].str[s[1]]);
	if (!new_b[0][0].str)
		return (free_t_block_tab(new_b), NULL);
	c[0] = s[0] + 1;
	while (c[0] != e[0])
	{
		new_b[0][0].str = ft_strcat_mal(new_b[0][0].str, block[c[0]].str);
		if (!new_b[0][0].str)
			return (free_t_block_tab(new_b), NULL);
		c[0] += 1;
	}
	if (save_last_line(block, e, c, new_b))
		return (NULL);
	if (wild)
		new_b[0] = split_on_wild(new_b, block, s);
	return (new_b);
}

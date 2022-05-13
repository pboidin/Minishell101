/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_mask.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:52:03 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 17:39:54 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_mask(t_block ***ret, int s[2], int e[2], char *wild)
{
	*ret = NULL;
	s[0] = 0;
	s[1] = 0;
	e[0] = 0;
	e[1] = 0;
	*wild = 0;
}

int	new_mask_block(int i[4], char *wild, t_block *block, t_block ***ret)
{
	t_block	**tmp;

	tmp = add_to_mask(i, &i[2], *wild, block);
	if (!tmp)
	{
		g_signal = 1;
		return (perror(" "), free_t_block_tab(*ret), 1);
	}
	*ret = add_block_to_tab(*ret, tmp);
	if (!*ret)
	{
		g_signal = 1;
		return (perror(" "), 1);
	}
	i[0] = i[2];
	i[1] = i[3] + 1;
	*wild = 0;
	return (0);
}

t_block	**build_mask(t_block *block)
{
	t_block	**ret;
	int		i[4];
	char	wild;

	init_mask(&ret, i, &i[2], &wild);
	while (block[i[2]].str)
	{
		i[3] = 0;
		while (block[i[2]].str[i[3]])
		{
			if (block[i[2]].str[i[3]] == '*' && block[i[2]].dbl_qu == 0
				&& block[i[2]].spl_qu == 0)
				wild = 1;
			if (block[i[2]].str[i[3]] == '/'
				&& new_mask_block(i, &wild, block, &ret))
				return (NULL);
			i[3] += 1;
		}
		i[2] += 1;
	}
	i[2] -= 1;
	i[3] = ft_strlen(block[i[2]].str);
	if (new_mask_block(i, &wild, block, &ret))
		return (NULL);
	return (ret);
}

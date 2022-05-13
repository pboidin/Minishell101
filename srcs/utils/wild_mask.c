/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_mask.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:52:03 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 15:57:06 by bdetune          ###   ########.fr       */
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

t_block	**build_mask(t_block *block)
{
	t_block	**ret;
	t_block	**tmp;
	int		s[2];
	int		e[2];
	char	wild;

	init_mask(&ret, s, e &wild);
	while (block[e[0]].str)
	{
		e[1] = 0;
		while (block[e[0]].str[e[1]])
		{
			if (block[e[0]].str[e[1]] == '*' && block[e[0]].dbl_qu == 0 && block[e[0]].spl_qu == 0)
				wild = 1;
			if (block[e[0]].str[e[1]] == '/')
			{
				tmp = add_to_mask(s, e, wild, block);
				if (!tmp)
					return (free_t_block_tab(ret), NULL);
				ret = add_block_to_tab(ret, tmp);
				if (!ret)
					return (NULL);
				s[0] = e[0];
				s[1] = e[1] + 1;
				wild = 0;
			}
			e[1] += 1;
		}
		e[0] += 1;
	}
	e[0] -= 1;
	e[1] = ft_strlen(block[e[0]].str);
	tmp = add_to_mask(s, e, wild, block);
	if (!tmp)
		return (free_t_block_tab(ret), NULL);
	ret = add_block_to_tab(ret, tmp);
	if (!ret)
		return (NULL);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 19:50:21 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 19:52:43 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_split(t_block **spl, int i[3])
{
	*spl = NULL;
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
}

void	jump_block(t_block *block, int s[2])
{
	if (block[s[0]].str[s[1]] == '\0')
	{
		s[0] += 1;
		s[1] = 0;
	}
}

int	update_wild_block(t_block **spl, t_block **new_block, int i[3], int val)
{
	*spl = add_str_t_block(*spl, &new_block[0][0].str[i[1]], \
		(i[0] - i[1]), i[2]);
	if (!spl)
	{
		g_signal = 1;
		return (free_t_block_tab(new_block), 1);
	}
	i[2] = val;
	i[1] = i[0];
	return (0);
}

void	last_chck(t_block *spl)
{
	if (spl[0].str[0] == '\0')
		move_upward_t_block_str(spl, 0);
	spl[0].dbl_qu = -1;
}

t_block	*split_on_wild(t_block **new_block, t_block *block, int s[2])
{
	t_block	*spl;
	int		i[3];

	init_split(&spl, i);
	jump_block(block, s);
	while (new_block[0][0].str[i[0]])
	{
		if (new_block[0][0].str[i[0]] == '*' && block[s[0]].dbl_qu == 0
			&& block[s[0]].spl_qu == 0)
		{
			if (update_wild_block(&spl, new_block, i, 1))
				return (NULL);
		}
		else if (i[2])
		{
			if (update_wild_block(&spl, new_block, i, 0))
				return (NULL);
		}
		i[0] += 1;
		s[1] += 1;
		jump_block(block, s);
	}
	if (update_wild_block(&spl, new_block, i, 0))
		return (NULL);
	return (last_chck(spl), free(new_block[0][0].str), free(new_block[0]), spl);
}

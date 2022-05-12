/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_upward.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:41:05 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/12 11:15:30 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	move_upward(t_cmd *cmd, int i, int mv)
{
	while (cmd->cmd_args[i])
	{
		cmd->cmd_args[i - mv] = cmd->cmd_args[i];
		i++;
	}
	while (mv)
	{
		cmd->cmd_args[i - mv] = NULL;
		mv--;
	}
}

void	move_t_block_tab_upward(t_block **tab, size_t i, int mv)
{
	while (tab[i])
	{
		tab[i - mv] = tab[i];
		i++;
	}
	while (mv)
	{
		tab[i - mv] = NULL;
		mv--;
	}
}

void	move_upward_t_block_str(t_block *block, int i)
{
	int	j;

	free(block[i].str);
	j = i + 1;
	while (block[j].str)
	{
		block[j - 1] = block[j];
		j++;
	}
	block[j - 1] = block[j];
}

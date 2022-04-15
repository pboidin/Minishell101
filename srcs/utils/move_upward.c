/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_upward.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:41:05 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 22:23:08 by bdetune          ###   ########.fr       */
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

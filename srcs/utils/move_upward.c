/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_upward.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:41:05 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/08 13:41:27 by bdetune          ###   ########.fr       */
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

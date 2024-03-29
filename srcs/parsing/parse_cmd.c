/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:18:37 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 14:45:38 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reinit_cmd(t_cmd *cmd)
{
	cmd->fork = NULL;
	cmd->pipe = NULL;
	cmd->sub_cmd = NULL;
	cmd->redirections = NULL;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->cmd_name = NULL;
	cmd->cmd_args = NULL;
}

int	parse_cmd(t_cmd *cmd)
{
	int		ret;

	if (g_signal > 0)
		return (0);
	reinit_cmd(cmd);
	ret = parse_logical(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = parse_pipe(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = fork_cmd(cmd);
	if (ret == 2)
		return (1);
	else if (ret)
		return (0);
	ret = parse_simple_cmd(cmd);
	if (ret)
		return (1);
	return (0);
}

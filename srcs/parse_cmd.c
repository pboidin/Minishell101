/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:18:37 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/21 12:28:31 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	reinit_cmd(t_cmd *cmd)
{
//	char	*trimmed_cmd;

//	trimmed_cmd = ft_trim(cmd->cmd);
//	if (!trimmed_cmd)
//		return (write(2, "Parsing error\n", 14), 1);
//	free(cmd->cmd);
//	cmd->cmd = trimmed_cmd;
	cmd->fork = NULL;
	cmd->pipe = NULL;
	cmd->sub_cmd = NULL;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->cmd_name = NULL;
	cmd->cmd_args = NULL;
	return (0);
}

int	parse_cmd(t_cmd *cmd)
{
	int		ret;

	if (reinit_cmd(cmd))
		return (1);
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

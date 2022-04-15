/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:47 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/11 23:29:38 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_blt(t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd_args[0], "echo") == 0
		|| ft_strcmp(cmd->cmd_args[0], "pwd") == 0
		|| ft_strcmp(cmd->cmd_args[0], "cd") == 0
		|| ft_strcmp(cmd->cmd_args[0], "env") == 0
		|| ft_strcmp(cmd->cmd_args[0], "unset") == 0
		|| ft_strcmp(cmd->cmd_args[0], "export") == 0
		|| ft_strcmp(cmd->cmd_args[0], "exit") == 0)
		return (0);
	return (1);
}

void	ft_blti(t_info *info, t_cmd *cmd)
{
	if (cmd->in)
	{
		close(cmd->in->fd);
		cmd->in->fd = -1;
	}
	if (ft_strcmp(cmd->cmd_args[0], "echo") == 0)
		info->status = ft_echo(cmd->cmd_args, cmd);
	else if (ft_strcmp(cmd->cmd_args[0], "cd") == 0)
		info->status = ft_ch_dir(cmd->cmd_args, info);
	else if (ft_strcmp(cmd->cmd_args[0], "env") == 0)
	 	info->status = ft_env(cmd->cmd_args, info, cmd);
	else if (ft_strcmp(cmd->cmd_args[0], "exit") == 0)
		info->status = ft_exit(cmd->cmd_args, info);
	else if (ft_strcmp(cmd->cmd_args[0], "export") == 0)
		info->status = ft_export(cmd->cmd_args, info);
	else if (ft_strcmp(cmd->cmd_args[0], "pwd") == 0)
		info->status = ft_working_dir(cmd->cmd_args, info, cmd);
	else if (ft_strcmp(cmd->cmd_args[0], "unset") == 0)
		info->status = ft_unset(cmd->cmd_args, info);
	if (cmd->out)
	{
		close(cmd->out->fd);
		cmd->out->fd = -1;
	}
}

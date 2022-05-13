/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 17:31:00 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	logical_controller(t_info *info, t_cmd *cmd)
{
	size_t	i;

	i = 0;
	while (cmd->sub_cmd[i])
	{
		if (i == 0)
			general_controller(info, cmd->sub_cmd[i]);
		else
		{
			if (cmd->sub_cmd[i]->prev_delim == AND && !info->status)
				general_controller(info, cmd->sub_cmd[i]);
			else if (cmd->sub_cmd[i]->prev_delim == OR && info->status)
				general_controller(info, cmd->sub_cmd[i]);
		}
		i++;
	}
}

static void	pipe_controller(t_info *info, t_cmd *cmd)
{
	int		fd[3];
	size_t	i;
	int		ret;

	i = 0;
	while (cmd->pipe[i])
	{
		if (cmd->pipe[i + 1] && pipe(fd))
			sys_call_error(info);
		ret = fork();
		if (ret == -1)
			sys_call_error(info);
		else if (!ret)
			pipe_child(info, cmd, i, fd);
		else
		{
			if (add_pid(info, ret))
				sys_call_error(info);
			fd[2] = pipe_parent(cmd, i, fd);
		}
		i++;
	}
	get_exit_status(info);
}

void	fork_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	ret = fork();
	if (ret == -1)
		sys_call_error(info);
	if (!ret)
		fork_child(info, cmd);
	else
	{
		if (add_pid(info, ret))
			sys_call_error(info);
		get_exit_status(info);
	}
}

int	simple_controller(t_info *info, t_cmd *cmd)
{
	if (handle_redirections(cmd, info))
		return (1);
	if (!cmd->cmd_args[0])
		return (0);
	if (is_assignation(cmd->cmd_args[0]))
		return (handle_assignation(cmd, info), 0);
	if (!ft_strcmp("export", cmd->cmd_args[0]))
	{
		if (export_expansion(info, cmd))
			return (1);
	}
	else
	{
		if (get_final_cmd(cmd, info))
			return (1);
	}
	if (!cmd->cmd_args[0])
		return (0);
	if (ft_blt(cmd) == 0)
		return (ft_blti(info, cmd), info->status);
	simple_cmd_child(info, cmd);
	return (info->status);
}

void	general_controller(t_info *info, t_cmd *cmd)
{
	if (g_signal > 0)
	{
		info->status = g_signal;
		return ;
	}
	if (cmd->sub_cmd)
		logical_controller(info, cmd);
	else if (cmd->pipe)
		pipe_controller(info, cmd);
	else if (cmd->fork)
		fork_controller(info, cmd);
	else
		info->status = simple_controller(info, cmd);
}

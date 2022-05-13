/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 22:25:03 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 23:35:53 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	simple_cmd_parent(t_info *info, t_cmd *cmd, int pid)
{
	if (cmd->in)
	{
		close(cmd->in->fd);
		cmd->in->fd = -1;
	}
	if (cmd->out)
	{
		close(cmd->out->fd);
		cmd->out->fd = -1;
	}
	if (add_pid(info, pid))
		sys_call_error(info);
	get_exit_status(info);
}

void	simple_cmd_child(t_info *info, t_cmd *cmd)
{
	int	ret;

	ret = fork();
	if (ret == -1)
		sys_call_error(info);
	if (!ret)
	{
		signal(SIGINT, child_signal);
		signal(SIGQUIT, child_signal);
		free_pid(info);
		if (cmd->in)
			dup2(cmd->in->fd, 0);
		if (cmd->out)
			dup2(cmd->out->fd, 1);
		ft_execute(info, cmd);
	}
	else
		simple_cmd_parent(info, cmd, ret);
}

void	pipe_child(t_info *info, t_cmd *cmd, size_t i, int fd[3])
{
	signal(SIGINT, child_signal);
	signal(SIGQUIT, child_signal);
	free_pid(info);
	info->is_child = TRUE;
	if (i == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
	}
	else if (!cmd->pipe[i + 1])
		dup2(fd[2], 0);
	else
	{
		dup2(fd[2], 0);
		dup2(fd[1], 1);
		close(fd[0]);
	}
	general_controller(info, cmd->pipe[i]);
	free_info(info);
	exit (info->status);
}

int	pipe_parent(t_cmd *cmd, size_t i, int fd[3])
{
	if (i == 0)
	{
		close(fd[1]);
		return (fd[0]);
	}
	else if (!cmd->pipe[i + 1])
	{
		close(fd[2]);
		return (-1);
	}
	else
	{
		close(fd[2]);
		close(fd[1]);
		return (fd[0]);
	}
}

void	fork_child(t_info *info, t_cmd *cmd)
{
	signal(SIGINT, child_signal);
	signal(SIGQUIT, child_signal);
	free_pid(info);
	info->is_child = TRUE;
	if (!handle_redirections(cmd, info))
	{
		if (cmd->in)
			dup2(cmd->in->fd, 0);
		if (cmd->out)
			dup2(cmd->out->fd, 1);
		general_controller(info, cmd->fork);
	}
	else
		info->status = 1;
	free_info(info);
	exit(info->status);
}

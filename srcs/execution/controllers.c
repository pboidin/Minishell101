/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 13:15:20 by bdetune          ###   ########.fr       */
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

/*
char	**replace_var(t_block *words, size_t i, t_info *info)
{
	char	**var_val;
	char	*var_found;

	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
	{
		var_val = (char **)ft_calloc(2, sizeof(char *));
		if (!var_val)
			return (NULL);
		if (words[i].str[1] == '?')
			var_val[0] = ft_itoa(info->status);
		else if (words[i + 1].str && (words[i + 1].str[0] == '"'
				|| words[i + 1].str[0] == 39))
			var_val[0] = (char *)ft_calloc(1, sizeof(char));
		else
			var_val[0] = ft_strdup(words[i].str);
		if (!var_val[0])
			return (free(var_val), NULL);
	}
	else
	{
		var_found = find_var(words[i].str, info);
		if (!var_found)
			return (NULL);
		if (!ft_strlen(var_found))
		{
			var_val = (char **)ft_calloc(2, sizeof(char *));
			if (!var_val)
				return (free(var_found), NULL);
			var_val[0] = var_found;
		}
		else
		{
			var_val = ft_split_charset(var_found, "\t\n\r\v\f ");
			free(var_found);
			if (!var_val)
				return (NULL);
		}
	}
	return (var_val);
}*/

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
	if (get_final_cmd(cmd, info))
		return (1);
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

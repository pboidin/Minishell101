/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:19:29 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 18:10:23 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirect(t_cmd *cmd)
{
	t_redirect	*current;
	t_redirect	*next;

	current = cmd->redirections;
	while (current)
	{
		next = current->next;
		free(current->str);
		if (current->fd != -1)
			close(current->fd);
		if (current->type == -2)
			unlink(current->path);
		free(current->path);
		free(current);
		current = next;
	}
	cmd->redirections = NULL;
}

void	free_t_cmd_tab(t_cmd **cmd_tab)
{
	size_t	i;

	if (!cmd_tab)
		return ;
	i = 0;
	while (cmd_tab[i])
	{
		free_cmd(cmd_tab[i]);
		free(cmd_tab[i]);
		i++;
	}
	free(cmd_tab);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->cmd);
	if (cmd->sub_cmd)
		free_t_cmd_tab(cmd->sub_cmd);
	else if (cmd->pipe)
		free_t_cmd_tab(cmd->pipe);
	else if (cmd->fork)
	{
		free_cmd(cmd->fork);
		free(cmd->fork);
	}
	if (cmd->cmd_args)
		free_char_tab(cmd->cmd_args);
	if (cmd->cmd_name)
		free(cmd->cmd_name);
	if (cmd->joined_env)
		free_char_tab(cmd->joined_env);
	free_redirect(cmd);
}

void	free_info(t_info *info)
{
	free_env(info);
	free_var(info);
	free_running_processes(info);
	free_cmd(&info->cmd);
	rl_clear_history();
}

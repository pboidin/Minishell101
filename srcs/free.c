/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:19:29 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/21 12:21:54 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_info *info)
{
	t_env	*current;
	t_env	*next;

	current = info->env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	info->env = NULL;
}

void	free_var(t_info *info)
{
	t_var	*current;
	t_var	*next;

	current = info->local_var;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	info->local_var = NULL;
}

void	free_running_processes(t_info *info)
{
	t_pid	*current;
	t_pid	*next;

	current = info->running_processes;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	info->running_processes = NULL;
}

void	free_redirect(t_cmd *cmd)
{
	t_redirect	*current;
	t_redirect	*next;

	current = cmd->in;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	current = cmd->out;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
}


void	free_cmd(t_cmd *cmd)
{
	size_t	i;
	t_cmd	**tab;

	tab = NULL;
	if (!cmd)
		return ;
	i = 0;
	if (cmd->sub_cmd)
		tab = cmd->sub_cmd;
	else if (cmd->pipe)
		tab = cmd->pipe;
	if (tab)
	{
		while (tab[i])
		{
			free_cmd(tab[i]);
			free(tab[i]);
			i++;
		}
		free(tab);
	}
	else if (cmd->fork)
	{
		free_cmd(cmd->fork);
		free(cmd->fork);
	}
	else if (cmd->cmd_args)
	{
		while (cmd->cmd_args[i])
		{
			free(cmd->cmd_args[i]);
			i++;
		}
		free(cmd->cmd_args);
	}
	if (cmd->cmd_name)
		free(cmd->cmd_name);
	free(cmd->cmd);
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

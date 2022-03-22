/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 14:05:02 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/21 14:14:15 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pid(t_info *info)
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

int	add_pid(t_info *info, int pid)
{
	t_pid	*new_pid;
	t_pid	*current;

	new_pid = (t_pid *)malloc(sizeof(t_pid));
	if (!new_pid)
		return (write(2, "Malloc error\n", 13), free_info(info), 1);
	new_pid->pid = pid;
	new_pid->next = NULL;
	current = info->running_processes;
	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = new_pid;
	}
	else
		info->running_processes = new_pid;
	return (0);
}

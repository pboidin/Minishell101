/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 12:10:15 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/11 12:11:18 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_exit_status(t_info *info)
{
	int		status;
	t_pid	*current;

	current = info->running_processes;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			info->status = WEXITSTATUS(status);
		current = current->next;
	}
	free_pid(info);
}

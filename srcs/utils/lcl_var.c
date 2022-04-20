/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcl_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 11:52:25 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 11:52:26 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*find_lcl_var(t_info *info, char *var_name)
{
	t_var	*current;

	current = info->local_var;
	while (current)
	{
		if (!ft_strcmp(current->name, var_name))
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_lcl_var(t_info *info, char *var[2])
{
	t_var	*new_var;

	new_var = ft_calloc(1, sizeof(t_var));
	if (!new_var)
	{
		free(var[0]);
		free(var[1]);
		sys_call_error(info);
	}
	new_var->name = var[0];
	new_var->value = var[1];
	new_var->next = info->local_var;
	info->local_var = new_var;
}

void	update_lcl_var(char *var[2], t_var *target)
{
	free(target->value);
	target->value = var[1];
	free(var[0]);
}

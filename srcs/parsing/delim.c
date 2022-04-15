/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:28:57 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 14:53:05 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delim(char *str, int delim)
{
	if (delim == 2 && ((*str == '|' && *(str + 1) == '|')
			|| (*str == '&' && *(str + 1) == '&')))
		return (1);
	else if (delim == 1 && *str == '|')
		return (1);
	else if (delim == 0 && (*str == '|' || *str == '&'))
		return (1);
	return (0);
}

void	save_delim(t_cmd *new_cmd, char *c, int prev_delim, int next_delim)
{
	if (!c)
		return ;
	if (!c[0])
		new_cmd->next_delim = next_delim;
	else if (c[0] == '|')
	{
		new_cmd->next_delim = PIPE;
		if (c[1] == '|')
			new_cmd->next_delim = OR;
	}
	else if (c[0] == '&' && c[1] == '&')
		new_cmd->next_delim = AND;
	else if (c[0] == ')')
		new_cmd->next_delim = PARENTH;
	new_cmd->prev_delim = prev_delim;
}

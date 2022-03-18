/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:28:57 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 16:39:27 by bdetune          ###   ########.fr       */
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

void	save_delim(t_cmd *new_cmd, char c)
{
	if (!c)
		new_cmd->next_delim = 0;
	else if (c == '|')
		new_cmd->next_delim = 3;
	else
		new_cmd->next_delim = 2;
}

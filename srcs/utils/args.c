/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 17:33:50 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 19:32:08 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_previous_args(t_cmd *cmd, int *i)
{
	int	j;

	j = 0;
	while (j < *i)
	{
		free(cmd->cmd_args[j]);
		j++;
	}
	move_upward(cmd, *i, *i);
	*i = 0;
	return (1);
}

int	is_valid_arg(char *str)
{
	size_t	i;
	int		spl_qu;
	int		dbl_qu;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[i] == '(' || str[i] == ')'
				|| str[i] == '|' || (str[i] == '&' && str[i + 1] == '&')
				|| str[i] == '<' || str[i] == '>'))
			return (0);
		i++;
	}
	return (1);
}

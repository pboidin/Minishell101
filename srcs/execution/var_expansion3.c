/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 12:20:07 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/15 12:20:47 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_var(t_block *word)
{
	size_t	i;
	size_t	tot_len;
	size_t	var_only;

	i = 0;
	tot_len = 0;
	var_only = 0;
	while (word[i].str)
	{
		tot_len += ft_strlen(word[i].str);
		var_only += word[i].var;
		i++;
	}
	if (!tot_len && var_only == i)
		return (1);
	return (0);
}

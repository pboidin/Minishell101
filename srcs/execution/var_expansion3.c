/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 12:20:07 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 15:46:04 by bdetune          ###   ########.fr       */
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

static char	**one_char_var_replace(t_block *words, size_t i, t_info *info)
{
	char	**var_val;

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
	return (var_val);
}

char	**replace_var(t_block *words, size_t i, t_info *info)
{
	char	**var_val;
	char	*var_found;

	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
		return (one_char_var_replace(words, i, info));
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
		}
	}
	return (var_val);
}

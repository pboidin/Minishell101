/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 12:12:52 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/11 12:23:59 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_englobing_char(char *str, size_t *i, char delim)
{
	*i += 1;
	while (str[*i] && str[*i] != delim)
		*i += 1;
	*i += 1;
}

static void	skip_to_end_var(char *str, size_t *i)
{
	if (str[*i + 1] == '?' || ft_isdigit(str[*i + 1]))
		*i += 2;
	else
	{
		*i += 1;
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			*i += 1;
	}
}

size_t	count_words_var_expansion(char *str)
{
	size_t	nb_words;
	size_t	i;

	i = 0;
	nb_words = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == 39)
			skip_englobing_char(str, &i, str[i]);
		else if (str[i] == '$' && str[i] != '\0')
			skip_to_end_var(str, &i);
		else
		{
			while (str[i] && !(str[i] == '$' || str[i] == 39 || str[i] == '"'))
				i++;
			if (str[i] == '$' && str[i + 1] == '\0')
				i++;
		}
		nb_words++;
	}
	return (nb_words);
}

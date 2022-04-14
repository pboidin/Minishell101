/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 12:12:52 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 21:58:30 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_englobing_char(char *str, size_t *i, char delim)
{
	*i += 1;
	while (str[*i] && str[*i] != delim)
		*i += 1;
	*i += 1;
}

void	skip_to_end_var(char *str, size_t *i)
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
		}
		nb_words++;
	}
	return (nb_words);
}

int	expand_var(t_info *info, t_block ***words_tab, size_t *j, size_t *i)
{
	size_t	word_count;
	char	**var;

	words_tab[0][*j][*i].var = 1;
	var = replace_var(words_tab[0][*j], *i, info);
	if (!var || !var[0])
		return (free(var), 1);
	if (char_tab_size(var) == 1)
	{
		free(words_tab[0][*j][*i].str);
		words_tab[0][*j][*i].str = var[0];
		free(var);
		return (0);
	}
	word_count = split_tab_var(words_tab, *j, *i, var);
	if (!word_count)
		return (1);
	*j += (word_count - 1);
	*i = 0;
	return (0);
}

char	*find_var(char *var, t_info *info)
{
	char	*value;
	t_env	*current_env;
	t_var	*current_var;

	current_env = info->env;
	while (current_env)
	{
		if (!ft_strcmp(&var[1], current_env->name))
		{
			value = ft_strdup(current_env->value);
			return (value);
		}
		current_env = current_env->next;
	}
	current_var = info->local_var;
	while (current_var)
	{
		if (!ft_strcmp(&var[1], current_var->name))
		{
			value = ft_strdup(current_var->value);
			return (value);
		}
		current_var = current_var->next;
	}
	return ((char *)ft_calloc(1, sizeof(char)));
}

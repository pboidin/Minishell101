/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 19:27:33 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 18:19:40 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_t_block_str(char *str, size_t *index)
{
	char	*word;
	size_t	i;
	size_t	j;

	i = *index;
	if (str[i] == '"' || str[i] == 39)
		skip_englobing_char(str, &i, str[i]);
	else if (str[i] == '$' && str[i] != '\0')
		skip_to_end_var(str, &i);
	else
	{
		while (str[i] && !(str[i] == '$' || str[i] == 39 || str[i] == '"'))
			i++;
	}
	word = (char *)ft_calloc((i - *index + 2), sizeof(char));
	if (!word)
		return (NULL);
	j = 0;
	while (*index < i)
	{
		word[j] = str[*index];
		j++;
		*index += 1;
	}
	return (word);
}

t_block	**create_t_tab(char *str)
{
	size_t	word_count;
	size_t	index;
	size_t	i;
	t_block	**words_tab;

	word_count = 0;
	words_tab = (t_block **)ft_calloc(2, sizeof(t_block *));
	if (!words_tab)
		return (NULL);
	word_count = count_words_var_expansion(str);
	words_tab[0] = (t_block *)ft_calloc((word_count + 1), sizeof(t_block));
	if (!words_tab[0])
		return (free_t_block_tab(words_tab), NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		words_tab[0][i].str = add_t_block_str(str, &index);
		if (!words_tab[0][i].str)
			return (free_t_block_tab(words_tab), NULL);
		i++;
	}
	return (words_tab);
}

int	remove_qu(t_block *tab, size_t i)
{
	size_t	j;
	size_t	len;
	char	*word;

	len = ft_strlen(tab[i].str);
	word = (char *)ft_calloc((len), sizeof(char));
	if (!word)
		return (1);
	j = 0;
	while (j < (len - 2))
	{
		word[j] = tab[i].str[j + 1];
		j++;
	}
	free(tab[i].str);
	tab[i].str = word;
	return (0);
}

int	handle_qu(t_info *info, t_block *word, size_t i, int expand)
{
	if (word[i].str[0] == 39)
	{
		word[i].spl_qu = 1;
		if (remove_qu(word, i))
			return (1);
	}
	else if (word[i].str[0] == '"')
	{
		word[i].dbl_qu = 1;
		if ((expand && inline_expansion(word, i, info))
			|| remove_qu(word, i))
			return (1);
	}
	return (0);
}

t_block	**add_args_word(char *str, t_info *info, int expand)
{
	t_block	**wt;
	size_t	i[2];

	wt = create_t_tab(str);
	if (!wt)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	while (wt[i[0]][i[1]].str)
	{
		if (wt[i[0]][i[1]].str[0] == '$' && expand)
		{
			if (expand_var(info, &wt, i, expand))
				return (free_t_block_tab(wt), NULL);
		}
		else if (wt[i[0]][i[1]].str[0] == 39 || wt[i[0]][i[1]].str[0] == '"')
		{
			if (handle_qu(info, wt[i[0]], i[1], expand))
				return (free_t_block_tab(wt), NULL);
		}
		i[1] += 1;
	}
	if (expand == 1)
		wt = replace_wild_cards(wt);
	return (wt);
}

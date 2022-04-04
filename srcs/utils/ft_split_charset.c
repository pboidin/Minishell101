/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 14:39:30 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/04 15:28:00 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_charset(char s, char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (s == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_count_word(char const *s, char *set)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && ft_is_charset(*s, set))
			s++;
		if (*s && !(ft_is_charset(*s, set)))
		{
			count++;
			while (*s && !(ft_is_charset(*s, set)))
				s++;
		}
	}
	return (count);
}

static char	*ft_malloc_word(char const *s, char *set)
{
	char	*word;
	int		i;

	i = 0;
	while (s[i] && !(ft_is_charset(s[i], set)))
		i++;
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!(word))
		return (NULL);
	i = 0;
	while (s[i] && !(ft_is_charset(s[i], set)))
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_charset(char const *s, char *set)
{
	char	**tab;
	int		i;
	int		count;

	i = 0;
	tab = (char **)malloc(sizeof(char *) * (ft_count_word(s, set) + 1));
	if (!tab)
		return (NULL);
	count = ft_count_word(s, set);
	while (*s && (i < count))
	{
		while (*s && ft_is_charset(*s, set))
			s++;
		if (*s && !(ft_is_charset(*s, set)))
		{
			tab[i] = ft_malloc_word(s, set);
			s++;
			while (*s && !(ft_is_charset(*s, set)))
				s++;
		}
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

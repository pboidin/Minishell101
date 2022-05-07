/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:58:43 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/02 16:02:21 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstlen(t_wild *list, t_block *str)
{
	int	i;

	i = 0;
	while (list)
	{
		if (ft_compare(list->path, str, 0, 0) == 1)
			i++;
		list = list->next;
	}
	return (i);
}

t_wild	*ft_lstlast_wild(t_wild *list)
{
	t_wild	*current;

	if (!list)
		return (NULL);
	current = list;
	while (current->next)
		current = current->next;
	return (current);
}

t_wild	*ft_lstnew_wild(void *content)
{
	t_wild	*new_el;

	new_el = malloc(sizeof(t_wild));
	if (!new_el)
		return (NULL);
	new_el->path = content;
	new_el->next = NULL;
	return (new_el);
}

int	ft_has_wildcards(t_block *block)
{
	int	i;

	i = 0;
	while (block[i].str)
	{
		if (block[i].dbl_qu != 0 && block[i].spl_qu != 0)
		{
			if (ft_strchr_wild(block[i].str, '*'))
				return (1);
		}
		i++;
	}
	return (0);
}

char	*ft_delete_wild(t_block *src)
{
	int		i;
	int		j;
	int		k;
	char	*dst;

	i = 0;
	j = 0;
	k = 0;
	while (src[i].str)
	{
		j = 0;
		while (src[i].str[j] != '*' && src[i].str[j] != '\0'))
		{
			k++;
			j++;
		}
		i++;
	}
	dst = malloc(sizeof(char) * (k + 1));
	if (!dst)
		return (NULL);
	i = 0;
	k = 0;
	while (src[i].str)
	{
		j = 0;
		while (src[i].str[j] != '*' && src[i].str[j] != '\0'))
		{
			dest[k] = src[i].str[j];
			k++;
			j++;
		}
		i++;
	}
	dst[k] = '\0';
	return (dst);
}

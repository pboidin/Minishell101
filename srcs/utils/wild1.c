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

int	ft_lstlen(t_wild *list, char *str)
{
	int	i;

	i = 0;
	while (list)
	{
		if (ft_compare(list->path, str) == 1)
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

int	ft_has_wildcards(char *str)
{
	if (ft_strchr_wild(str, '\'') == 1 || ft_strchr_wild(str, '\"') == 1)
		return (0);
	if (ft_strchr_wild(str, '*'))
		return (1);
	return (0);
}

char	*ft_delete_wild(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '*' && src[i] != '\0')
		i++;
	dst = malloc(sizeof(char) * (i + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (src[i] != '*' && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

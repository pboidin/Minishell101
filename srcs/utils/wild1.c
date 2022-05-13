/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:58:43 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/13 14:48:50 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstlen(t_wild *list)
{
	int	i;

	i = 0;
	while (list)
	{
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
		if (block[i].dbl_qu == 0 && block[i].spl_qu == 0)
		{
			if (ft_strchr_wild(block[i].str, '*'))
				return (1);
		}
		i++;
	}
	return (0);
}

t_wild	*add_to_list(t_wild *list, char *path, t_block **mask)
{
	t_wild	*last;
	t_wild	*new_block;

	if (!path)
		return (free_t_wild(list), NULL);
	if (mask[1])
	{
		path = ft_strcat_mal(path, "/");
		if (!path)
		{
			g_signal = 1;
			return (perror("malloc error"), free_t_wild(list), NULL);
		}
	}
	new_block = ft_lstnew_wild(path);
	if (!new_block)
	{
		g_signal = 1;
		return (perror("malloc error"), free(path), free_t_wild(list), NULL);
	}
	if (!list)
		return (new_block);
	last = ft_lstlast_wild(list);
	last->next = new_block;
	return (list);
}

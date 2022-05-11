/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:58:43 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/11 18:31:32 by bdetune          ###   ########.fr       */
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
		if (block[i].dbl_qu == 0 && block[i].spl_qu == 0)
		{
			if (ft_strchr_wild(block[i].str, '*'))
				return (1);
		}
		i++;
	}
	return (0);
}

char	*ft_delete_wild(t_block *src, int *depth)
{
	int		i;
	int		j;
	int		k;
	int		tmp;
	char	*dst;

	i = 0;
	j = 0;
	*depth = 0;
	while (src[i].str)
	{
		j = 0;
		while (!(src[i].str[j] == '*' && src[i].dbl_qu == 0
				&& src[i].spl_qu == 0) && src[i].str[j] != '\0')
		{
			if (src[i].str[j] == '/')
				*depth += 1;
			j++;
		}
		if (src[i].str[j] == '*')
			break;
		i++;
	}
	if (*depth == 0)
	{
		dst = ft_calloc(2, sizeof(char));
		if (!dst)
			return (NULL);
		dst[0] = '.';
	}
	else
	{
		tmp = 0;
		k = 0;
		i = 0;
		while (tmp != *depth)
		{
			j = 0;
			while (src[i].str[j] != '\0')
			{
				k++;
				if (src[i].str[j] == '/')
				{
					tmp++;
					if (tmp == *depth)
						break ;
				}
				j++;
			}
			i++;
		}
		if (k == 1)
		{
			dst = ft_calloc(2, sizeof(char));
			if (!dst)
				return (NULL);
			dst[0] = '/';
			j++;
			i--;
		}
		else
		{
			dst = ft_calloc((k), sizeof(char));
			if (!dst)
				return (NULL);
			tmp = 0;
			i = 0;
			while (src[i].str)
			{
				j = 0;
				while (src[i].str[j])
				{
					dst[tmp] = src[i].str[j];
					tmp++;
					j++;
					if (tmp == (k - 1))
						break ;
				}
				if (tmp == (k - 1))
					break ;
				i++;
			}
			if (src[i].str[j] == '/')
				j++;
			else
			{
				i++;
				while (src[i].str[0] != '/')
					i++;
				j = 1;
			}
		}
	}
	if (*depth == 0)
		*depth = 1;
	else
		*depth = 0;
	while (src[i].str[j])
	{
		if (src[i].str[j] == '/')
			*depth += 1;
		j++;
	}
	i++;
	while (src[i].str)
	{
		j = 0;
		while (src[i].str[j])
		{
			if (src[i].str[j] == '/')
				*depth += 1;
			j++;
		}
		i++;
	}
	return (dst);
}

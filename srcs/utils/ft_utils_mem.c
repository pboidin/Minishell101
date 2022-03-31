/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 15:43:55 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 15:44:36 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dst, const void *src, size_t memSize)
{
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	tmp1 = (unsigned char *)src;
	tmp2 = (unsigned char *)dst;
	while (memSize > 0)
	{
		*tmp2++ = *tmp1++;
		memSize--;
	}
	return (dst);
}

void	*ft_realloc(void *ptr, size_t memSize)
{
	void	*new;

	if (!ptr)
		return (malloc(memSize));
	if (!memSize)
	{
		if (ptr)
		{
			free (ptr);
			ptr = NULL;
		}
		return (NULL);
	}
	new = malloc(memSize);
	if (!new)
		return (NULL);
	ft_memcpy(new, ptr, memSize);
	free (ptr);
	ptr = NULL;
	return (new);
}

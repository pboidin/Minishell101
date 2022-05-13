/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 12:35:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 14:49:18 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest && dest[i] != '\0')
		i++;
	j = 0;
	while (src && src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void	delete_empty(t_block *block)
{
	int	i;

	i = 0;
	while (block[i].str)
	{
		if (block[i].str[0] == '\0')
		{
			move_upward_t_block_str(block, i);
			i--;
		}
		i++;
	}
}

int	end_wilderness(t_block **mask)
{
	if (!mask[1] || (mask[1][0].str[0] == '\0' && !mask[2]))
		return (1);
	return (0);
}

int	final_match(char *name, char *full_path, t_block **mask)
{
	struct stat	fstat;

	if (!ft_compare(mask[0], name))
		return (0);
	if (mask[1])
	{
		if (!(stat(full_path, &fstat) < 0) && S_ISDIR(fstat.st_mode))
			return (1);
		else
			return (0);
	}
	return (1);
}

void	free_t_wild(t_wild *list)
{
	t_wild	*current;
	t_wild	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->path);
		free(current);
		current = next;
	}
}

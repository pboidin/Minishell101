/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_str3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 12:23:56 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/12 12:36:34 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcat_mal(char *str1, char *str2)
{
	int		i;
	char	*dst;
	size_t	total_size;

	if (str1)
		total_size = ft_strlen(str1);
	if (str2)
		total_size += ft_strlen(str2);
	dst = ft_calloc((total_size + 1), sizeof(char));
	if (!dst)
		return (free(str1), NULL);
	total_size = 0;
	i = -1;
	while (str1 && str1[++i])
	{
		dst[total_size] = str1[i];
		total_size++;
	}
	i = -1;
	while (str2 && str2[++i])
	{
		dst[total_size] = str2[i];
		total_size++;
	}
	return (free(str1), dst);
}

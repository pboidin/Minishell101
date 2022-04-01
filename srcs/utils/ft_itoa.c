/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 13:34:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/01 20:03:20 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	lcl_ft_nblen(int n)
{
	size_t			nb_len;
	unsigned int	x;

	nb_len = 0;
	if (n < 0)
	{
		x = -n;
		nb_len++;
	}
	else
		x = n;
	while (x >= 10)
	{
		nb_len++;
		x /= 10;
	}
	return (nb_len + 1);
}

char	*ft_itoa(int n)
{
	char			*nb;
	size_t			nb_len;
	unsigned int	unb;

	nb_len = lcl_ft_nblen(n);
	nb = (char *)ft_calloc((nb_len + 1), sizeof(char));
	if (!nb)
		return (NULL);
	if (n < 0)
		unb = -n;
	else
		unb = n;
	while (nb_len-- > 0)
	{
		nb[nb_len] = (unb % 10) + '0';
		unb /= 10;
	}
	if (n < 0)
		nb[0] = '-';
	return (nb);
}

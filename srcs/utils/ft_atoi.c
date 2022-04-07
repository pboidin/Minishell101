/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 20:08:12 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/07 13:03:16 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	cc;

	cc = (char)c;
	i = 0;
	while (s[i])
	{
		if (cc == s[i])
			return ((char *)&s[i]);
		i++;
	}
	if (cc == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

static size_t	lcl_ft_ulllen(unsigned long long n)
{
	size_t	nb_len;

	nb_len = 0;
	while (n >= 10)
	{
		nb_len++;
		n /= 10;
	}
	return (nb_len + 1);
}

static int	lcl_getsign(const char *str, size_t *i)
{
	int	sign_factor;

	sign_factor = 1;
	while (str[*i] && ft_strchr("\t\n\r\v\f ", str[*i]))
		*i += 1;
	if (str[*i] && str[*i] == '-')
	{
		sign_factor = -1;
		*i += 1;
	}
	else if (str[*i] && str[*i] == '+')
		*i += 1;
	return (sign_factor);
}

int	ft_atoi(const char *str)
{
	size_t				i;
	size_t				max_len;
	char				*nb_char;
	int					sign_factor;
	unsigned long long	ret;

	max_len = lcl_ft_ulllen(ULLONG_MAX);
	ret = 0;
	i = 0;
	sign_factor = lcl_getsign(str, &i);
	nb_char = (char *)&str[i];
	i = 0;
	while (nb_char[i] && ft_isdigit(nb_char[i]))
	{
		if ((i + 1) == max_len)
		{
			if (sign_factor < 0)
				return (0);
			else
				return ((int)ULLONG_MAX);
		}
		ret = ret * 10 + nb_char[i] - '0';
		i++;
	}
	return ((int)(sign_factor * ret));
}

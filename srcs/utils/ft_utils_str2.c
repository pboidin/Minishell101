/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_str2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:00:50 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/02 15:45:10 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_trim(char *cmd)
{
	int		i;
	int		j;
	int		z;
	char	*new_cmd;

	i = 0;
	skip_whitespaces(cmd, &i);
	if (!cmd || !cmd[i])
		return (NULL);
	j = i;
	while (cmd[j + 1])
		j++;
	while (j >= i
		&& ((cmd[i + j] >= '\t' && cmd[i + j] <= '\r') || cmd[i + j] == ' ' ))
		j--;
	new_cmd = (char *)malloc(sizeof(char) * (j - i + 2));
	if (!new_cmd)
		return (NULL);
	z = 0;
	while (i <= j)
		new_cmd[z++] = cmd[i++];
	new_cmd[z] = '\0';
	return (new_cmd);
}

void	skip_whitespaces(char *cmd, int *i)
{
	if (!cmd)
		return ;
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
}

int	is_whitespace(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	return ;
}

char	*ft_strcp(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '\0')
		i++;
	dst = malloc(sizeof(char) * (i + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (src[i] && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

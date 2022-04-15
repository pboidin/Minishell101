/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 13:06:19 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/15 15:14:06 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*internal_get_str(int fd, char *remainer, int start_index, int *nl_pos)
{
	int		ret;
	char	*buffer;
	char	*new_remainer;

	if (internal_hasnl(remainer, &start_index, nl_pos))
		return (remainer);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret <= 0)
	{
		free (buffer);
		return (remainer);
	}
	buffer[ret] = '\0';
	new_remainer = internal_join(remainer, buffer);
	free(remainer);
	free(buffer);
	return (internal_get_str(fd, new_remainer, start_index, nl_pos));
}

int	get_next_line(int fd, char **line)
{
	static char	*remainer = NULL;
	char		*new_remainer;
	int			nl_pos;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (0);
	*line = NULL;
	nl_pos = -1;
	remainer = internal_get_str(fd, remainer, 0, &nl_pos);
	*line = internal_get_line(remainer, &nl_pos);
	new_remainer = internal_getremainer(remainer, (nl_pos + 1));
	free(remainer);
	remainer = new_remainer;
	if (!*line)
		return (0);
	return (1);
}

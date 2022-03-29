/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:16:27 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/29 14:20:25 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_del_spaces(char *str)
{
	size_t	size;
	size_t	i;
	char	*new_str;

	size = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			size++;
		i++;
	}
	new_str = (char *)malloc(sizeof(char) * (size + 1));
	if (!new_str)
		return (perror("Malloc error"), NULL);
	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			new_str[size] = str[i];
			size++;
		}
		i++;
	}
	new_str[size] = '\0';
	return (new_str);
}

int	get_delimiter(t_redirect *redir)
{
	int		spl_qu;
	int		dbl_qu;
	size_t	i;
	char	*word;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	redir->var_expansion = 1;
	while (redir->str[i])
	{
		if (redir->str[i] == 39 && !dbl_qu)
		{
			redir->str[i] = ' ';
			spl_qu ^= 1;
			redir->var_expansion = 0;
		}
		else if (redir->str[i] == '"' && !spl_qu)
		{
			redir->str[i] = ' ';
			dbl_qu ^= 1;
			redir->var_expansion = 0;
		}
		i++;
	}
	word = ft_del_spaces(redir->str);
	if (!word)
		return (1);
	free(redir->str);
	redir->str = word;
	printf("delimiter %s\n", redir->str);
	return (0);
}

void	init_nb(char *nb)
{
	nb[0] = '0';
	nb[1] = '0';
	nb[2] = '0';
	nb[3] = '0';
	nb[4] = '0';
	nb[5] = '0';
	nb[6] = '0';
	nb[7] = '0';
	nb[8] = '0';
	nb[9] = '1';
	nb[10] = '\0';
}

void	increase_nb(char *nb, int index)
{
	nb[index] += 1;
	if (nb[index] > '9')
	{
		if (index == 0)
			return ;
		nb[index] = '0';
		increase_nb(nb, (index - 1));
	}
}

int	save_heredoc(t_redirect *new_redirect)
{
	int		size;
	char	*ret;
	char	*path;
	char	nb[11];

	if (get_delimiter(new_redirect))
		return (1);
	size = ft_strlen(new_redirect->str) + 1;
	init_nb(nb);
	path = ft_strjoin("/tmp/minishell-", nb);
	while (!access(path, F_OK))
	{
		free(path);
		increase_nb(nb, 9);
		if (nb[0] > '9')
			return (write(2, "Error creating here-document\n", 29), 1);
		path = ft_strjoin("/tmp/minishell-", nb);
	}
	new_redirect->path = path;
	new_redirect->fd = open(path, O_CREAT | O_WRONLY, 0644);
	if (new_redirect->fd == -1)
		return (perror("Error creating here-document"), 1);
	ret = readline("> ");
	while (ret)
	{
		if (!ft_strncmp(ret, new_redirect->str, size))
			break ;
		write(new_redirect->fd, ret, ft_strlen(ret));
		write(new_redirect->fd, "\n", 1);
		free(ret);
		ret = readline("> ");
	}
	if (!ret)
	{
		write (2, \
		"Warning: here-document delimited by end-of-file (wanted `", 57);
		write(2, new_redirect->str, ft_strlen(new_redirect->str));
		write(2, "')\n", 3);
	}
	free(ret);
	close(new_redirect->fd);
	new_redirect->fd = -1;
	return (0);
}

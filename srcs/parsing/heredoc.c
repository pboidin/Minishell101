/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:16:27 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 12:56:49 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_delimiter(t_redirect *redir)
{
	size_t	i;
	t_block	**word_blocks;
	char	*word;

	word_blocks = add_args_word(redir->str, NULL, 0);
	if (!word_blocks || !word_blocks[0] || word_blocks[1])
		return (free_t_block_tab(word_blocks), 1);
	redir->var_expansion = 1;
	i = 0;
	while (word_blocks[0][i].str)
	{
		if (word_blocks[0][i].dbl_qu || word_blocks[0][i].spl_qu)
			redir->var_expansion = 0;
		i++;
	}
	word = t_block_to_str(word_blocks[0]);
	free_t_block_tab(word_blocks);
	if (!word)
		return (1);
	free(redir->str);
	redir->str = word;
	return (0);
}

void	write_to_heredoc(t_redirect *new_redirect)
{
	char	*ret;

	signal(SIGQUIT, SIG_IGN);
	g_signal = -2;
	ret = readline("> ");
	while (ret)
	{
		if (!ft_strcmp(ret, new_redirect->str))
			break ;
		write(new_redirect->fd, ret, ft_strlen(ret));
		write(new_redirect->fd, "\n", 1);
		free(ret);
		ret = readline("> ");
	}
	if (!ret && g_signal <= 0)
	{
		write (2, \
		"Warning: here-document delimited by end-of-file (wanted `", 57);
		write(2, new_redirect->str, ft_strlen(new_redirect->str));
		write(2, "')\n", 3);
		g_signal = 0;
	}
	signal(SIGQUIT, handle_signal);
	free(ret);
}

int	save_heredoc(t_redirect *new_redirect)
{
	if (get_delimiter(new_redirect))
		return (1);
	new_redirect->path = create_tmp();
	if (!new_redirect->path)
		return (1);
	new_redirect->fd = open(new_redirect->path, O_CREAT | O_WRONLY, 0644);
	if (new_redirect->fd == -1)
		return (perror("Error creating here-document"), 1);
	write_to_heredoc(new_redirect);
	close(new_redirect->fd);
	new_redirect->fd = -1;
	return (0);
}

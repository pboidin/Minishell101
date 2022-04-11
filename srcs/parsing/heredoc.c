/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:16:27 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/11 12:08:28 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_delimiter(t_redirect *redir)
{
	size_t	i;
	t_block	**word_blocks;
	char	*word;
	char	*tmp;

	word_blocks = add_args_word(redir->str, NULL, 0);
	if (!word_blocks || !word_blocks[0] || word_blocks[1])
		return (1);
	word = ft_strdup(word_blocks[0][0].str);
	if (!word)
		return (free_t_block_tab(word_blocks), 1);
	i = 1;
	redir->var_expansion = 1;
	while (word_blocks[0][i].str)
	{
		tmp = ft_strjoin(word, word_blocks[0][i].str);
		if (!tmp)
			return (perror("Malloc error"), free_t_block_tab(word_blocks), free(word), 1);
		free(word);
		word = tmp;
		if (word_blocks[0][i].dbl_qu || word_blocks[0][i].spl_qu)
			redir->var_expansion = 0;
		i++;
	}
	free(redir->str);
	redir->str = word;
	return (free_t_block_tab(word_blocks), 0);
}

int	save_heredoc(t_redirect *new_redirect)
{
	char	*ret;

	if (get_delimiter(new_redirect))
		return (1);
	new_redirect->path = create_tmp();
	if (!new_redirect->path)
		return (1);
	new_redirect->fd = open(new_redirect->path, O_CREAT | O_WRONLY, 0644);
	if (new_redirect->fd == -1)
		return (perror("Error creating here-document"), 1);
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

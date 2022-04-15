/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 12:05:52 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/15 14:00:40 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path(char *str, t_info *info)
{
	t_block	**path;
	char	*word;

	path = add_args_word(str, info, 1);
	if (!path)
		return (sys_call_error(info), NULL);
	if (t_block_tab_size(path) != 1 || is_empty_var(path[0]))
		return (ambiguous_redirect(str), free_t_block_tab(path), NULL);
	word = t_block_to_str(path[0]);
	free_t_block_tab(path);
	if (!word)
		return (sys_call_error(info), NULL);
	return (word);
}

static int	handle_in_redirections(t_cmd *cmd, t_redirect *current)
{
	if (cmd->in)
	{
		close(cmd->in->fd);
		cmd->in->fd = -1;
	}
	current->fd = open(current->path, O_RDONLY);
	if (current->fd == -1)
		return (open_error(current), EXIT_FAILURE);
	cmd->in = current;
	return (0);
}

static int	handle_out_redirections(t_cmd *cmd, t_redirect *current)
{
	if (cmd->out)
	{
		close(cmd->out->fd);
		cmd->out->fd = -1;
	}
	if (current->type == 1)
		current->fd = open(current->path, O_RDWR | O_TRUNC | O_CREAT, 0666);
	else
		current->fd = open(current->path, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (current->fd == -1)
		return (open_error(current), EXIT_FAILURE);
	cmd->out = current;
	return (0);
}

static int	heredoc_expand(t_info *info, t_redirect *current)
{
	t_block		word;
	t_redirect	new_redir;

	current->fd = open(current->path, O_RDONLY);
	if (current->fd == -1)
		return (open_error(current), EXIT_FAILURE);
	new_redir.path = create_tmp();
	if (!new_redir.path)
		return (1);
	new_redir.fd = open(new_redir.path, O_RDWR | O_CREAT, 0666);
	if (new_redir.fd == -1)
		return (open_error(&new_redir), free(new_redir.path), EXIT_FAILURE);
	while (get_next_line(current->fd, &word.str))
	{
		inline_expansion(&word, 0, info);
		write(new_redir.fd, word.str, ft_strlen(word.str));
		free(word.str);
	}
	close(current->fd);
	current->fd = -1;
	unlink(current->path);
	free(current->path);
	current->path = new_redir.path;
	return (close(new_redir.fd), 0);
}

int	handle_redirections(t_cmd *cmd, t_info *info)
{
	t_redirect	*current;

	current = cmd->redirections;
	while (current)
	{
		if (current->type != -2)
			current->path = get_path(current->str, info);
		else if (current->var_expansion && heredoc_expand(info, current))
			return (1);
		if (!current->path)
			return (1);
		if (current->type < 0)
		{
			if (handle_in_redirections(cmd, current))
				return (1);
		}
		else
		{
			if (handle_out_redirections(cmd, current))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

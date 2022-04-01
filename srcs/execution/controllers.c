/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/01 17:17:42 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	logical_controller(t_info *info, t_cmd *cmd)
{
	size_t	i;

	i = 0;
	while (cmd->sub_cmd[i])
	{
		if (i == 0)
			general_controller(info, cmd->sub_cmd[i]);
		else
		{
			if (cmd->sub_cmd[i]->prev_delim == AND && !info->status)
				general_controller(info, cmd->sub_cmd[i]);
			else if (cmd->sub_cmd[i]->prev_delim == OR && info->status)
				general_controller(info, cmd->sub_cmd[i]);
		}
		i++;
	}
}

void	get_exit_status(t_info *info)
{
	int		status;
	t_pid	*current;

	current = info->running_processes;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			info->status = WEXITSTATUS(status);
		current = current->next;
	}
	free_pid(info);
}

static void	pipe_controller(t_info *info, t_cmd *cmd)
{
	int		fd[2];
	size_t	i;
	int		ret;
	int		old_fd;

	i = 0;
	while (cmd->pipe[i])
	{
		if (cmd->pipe[i + 1])
		{
			if (pipe(fd))
			{
				write(2, "Pipe error\n", 11);
				free_info(info);
				exit (1);
			}
		}
		ret = fork();
		if (ret == -1)
		{
			write(2, "Fork error\n", 11);
			free_info(info);
			exit (1);
		}
		else if (!ret)
		{
			free_pid(info);
			if (i == 0)
			{
				dup2(fd[1], 1);
				close(fd[0]);
			}
			else if (!cmd->pipe[i + 1])
				dup2(old_fd, 0);
			else
			{
				dup2(old_fd, 0);
				dup2(fd[1], 1);
				close(fd[0]);
			}
			general_controller(info, cmd->pipe[i]);
			free_info(info);
			exit (info->status);
		}
		else
		{
			if (add_pid(info, ret))
				exit (1);
			if (i == 0)
			{
				close(fd[1]);
				old_fd = fd[0];
			}
			else if (!cmd->pipe[i + 1])
			   close(old_fd);
			else
			{
				old_fd = fd[0];
				close(fd[1]);
			}
		}
		i++;
	}
	get_exit_status(info);
}

size_t	count_words_var_expansion(char *str)
{
	size_t	nb_words;
	size_t	i;

	i = 0;
	nb_words = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
				i++;
			nb_words++;
		}
		else if(str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
			nb_words++;
		}
		else if (str[i] == '$' && str[i] != '\0')
		{
			nb_words++;
			if (str[i + 1] == '?')
				i++;
			else
			{
				i++;
				while (str[i] && !(str[i] == 39 || str[i] == '"' || str[i] == '$'))
					i++;
				if (str[i] == '\0')
					break ;
				else
					i--;
			}
		}
		else
		{
			nb_words++;
			while (str[i] && !(str[i] == '$' || str[i] == 39 || str[i] == '"'))
				i++;
			if (str[i] == '$' && str[i + 1] == '\0')
				i++;
			if (!str[i])
				break ;
			else
				i--;
		}
		i++;
	}
	return (nb_words);
}

char	*add_redirect_word(char *str, size_t *index)
{
	char	*word;
	size_t	i;
	size_t	j;

	i = *index;
	printf("Rest of word: %s\n", &str[*index]);
	if (str[i] == '"')
	{
		i++;
		while (str[i] && str[i] != '"')
			i++;
		i++;
	}
	else if(str[i] == 39)
	{
		i++;
		while (str[i] && str[i] != 39)
			i++;
		i++;
	}
	else if (str[i] == '$' && str[i] != '\0')
	{
		if (str[i + 1] == '?')
			i += 2;
		else
		{
			i++;
			while (str[i] && !(str[i] == 39 || str[i] == '"' || str[i] == '$'))
				i++;
		}
	}
	else
	{
		while (str[i] && !(str[i] == '$' || str[i] == 39 || str[i] == '"'))
			i++;
		if (str[i] == '$' && str[i + 1] == '\0')
			i++;
	}
	word = (char *)ft_calloc((i - *index + 1), sizeof(char));
	if (!word)
		return (perror("Malloc error"), NULL);
	j = 0;
	while (*index < i)
	{
		word[j] = str[*index];
		j++;
		*index += 1;
	}
	return (word);
}

char	**expand_redirect_var(char *str)
{
	size_t	word_count;
	char	**words;
	size_t	i;
	size_t	index;

	word_count = count_words_var_expansion(str);
	words = (char **)ft_calloc((word_count + 1), sizeof(char *));
	if (!words)
		return (perror("Malloc error"), NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		words[i] = add_redirect_word(str, &index);
		if (!words[i])
		{
			while (i--)
				free(words[i]);
			return (free(words), NULL);
		}
		i++;
	}
	i = 0;
	while (words[i])
	{

		printf("Word in redirection: %s\n", words[i]);
		i++;
	}
	return (words);
}

char *get_path(char *str)
{
//	char	**path;
	int		spl_qu;
	int		dbl_qu;
	size_t	i;
	char	*word;

	expand_redirect_var(str); //	path = expand_redirect_var(str);
	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
		{
			str[i] = ' ';
			spl_qu ^= 1;
		}
		else if (str[i] == '"' && !spl_qu)
		{
			str[i] = ' ';
			dbl_qu ^= 1;
		}
		i++;
	}
	word = ft_del_spaces(str);
	if (!word)
		return (NULL);
	return (word);
}

int	handle_redirections(t_cmd *cmd)
{
	t_redirect	*current;
	
	current = cmd->redirections;
	while (current)
	{
		if (current->type != -2)
			current->path = get_path(current->str);
		if (current->type < 0)
		{
			if (cmd->in)
			{
				close(cmd->in->fd);
				cmd->in->fd = -1;
			}
			current->fd = open(current->path, O_RDONLY);
			if (current->fd == -1)
				return (perror("Open error"), 1);
			cmd->in = current;
		}
		else
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
				return (perror("Open error"), 1);
			cmd->out = current;
		}
		current = current->next;
	}
	return (0);
}

void	fork_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	ret = fork();
	if (ret == -1)
	{
		write(2, "Fork error\n", 11);
		exit (1);
	}
	if (!ret)
	{
		free_pid(info);
		if (!handle_redirections(cmd))
		{
			if (cmd->in)
				dup2(cmd->in->fd, 0);
			if (cmd->out)
				dup2(cmd->out->fd, 1);
			general_controller(info, cmd->fork);
		}
		else
			info->status = 1;
		free_info(info);
		exit(info->status);
	}
	else
	{
		if (add_pid(info, ret))
		{
			write(2, "Malloc error\n", 13);
			free_info(info);
			exit (1);
		}
		get_exit_status(info);
	}
}

void	simple_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	ret = fork();
	if (ret == -1)
	{
		write(2, "Fork error\n", 13);
		exit (1);
	}
	if (!ret)
	{
		ft_execute(info, cmd->cmd_args);
	}
	else
	{
		if (add_pid(info, ret))
		{
			write(2, "Malloc error\n", 13);
			exit (1);
		}
		get_exit_status(info);
	}
}

void	general_controller(t_info *info, t_cmd *cmd)
{
	if (cmd->sub_cmd)
		logical_controller(info, cmd);
	else if (cmd->pipe)
		pipe_controller(info, cmd);
	else if (cmd->fork)
		fork_controller(info, cmd);
	else
		simple_controller(info, cmd);
}

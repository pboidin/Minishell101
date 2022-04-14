/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 18:11:50 by bdetune          ###   ########.fr       */
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
			info->is_child = TRUE;
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

char	*add_redirect_word(char *str, size_t *index)
{
	char	*word;
	size_t	i;
	size_t	j;

	i = *index;
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
		i++;
		if (str[i] == '?')
			i++;
		else if (ft_isdigit(str[i + 1]))
				i++;
		else
		{
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
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

char	**replace_redirect_var(t_block *words, size_t i, t_info *info)
{
	char	**var_val;
	char	*var_found;

	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
	{
		var_val = (char **)ft_calloc(2, sizeof(char *));
		if (!var_val)
			return (perror("malloc error"), NULL);
		if (words[i].str[1] == '?')
			var_val[0] = ft_itoa(info->status);
		else if (words[i + 1].str && (words[i + 1].str[0] == '"' || words[i + 1].str[0] == 39))
			var_val[0] = (char *)ft_calloc(1, sizeof(char));
		else
			var_val[0] = ft_strdup(words[i].str);
		if (!var_val[0])
			return (perror("Malloc error"), free(var_val), NULL);
	}
	else
	{
		var_found = find_var(words[i].str, info);
		if (!var_found)
			return (NULL);
		if (!ft_strlen(var_found))
		{
			var_val = (char **)ft_calloc(2, sizeof(char *));
			if (!var_val)
				return (free(var_found), perror("malloc error"), NULL);
			var_val[0] = var_found; 
		}
		else
		{
			var_val = ft_split_charset(var_found, "\t\n\r\v\f ");
			free(var_found);
			if (!var_val)
				return (perror("Malloc error"), NULL);
		}
	}
	return (var_val);
}

int	remove_qu(t_block *tab, size_t i)
{
	size_t	j;
	size_t	len;
	char	*word;

	len = ft_strlen(tab[i].str);
	word = (char *)ft_calloc((len - 1), sizeof(char));
	if (!word)
		return (perror("Malloc error"), 1);
	j = 0;
	while (j < (len - 2))
	{
		word[j] = tab[i].str[j + 1];
		j++;
	}
	free(tab[i].str);
	tab[i].str = word;
	return (0);
}

t_block	*expand_redirect_var(char *str, t_info *info)
{
	size_t	word_count;
	t_block	*words;
	size_t	i;
	size_t	index;
	char	**var;

	word_count = count_words_var_expansion(str);
	words = (t_block *)ft_calloc((word_count + 1), sizeof(t_block));
	if (!words)
		return (perror("Malloc error"), NULL);
	i = 0;
	index = 0;
	while (i < word_count)
	{
		words[i].str = add_redirect_word(str, &index);
		if (!words[i].str)
			return (free_t_block(words), NULL);
		i++;
	}
	i = 0;
	while (words[i].str)
	{
		if (words[i].str[0] == '$')
		{
			words[i].var = 1;
			var = replace_redirect_var(words, i, info);
			if (!var || !var[0])
			{
				i = 0;
				free_t_block(words);
				if (var)
					free(var);
				return (NULL);
			}
			else if (var[1])
			{
				write(2, str, ft_strlen(str));
				write(2, ": ambiguous redirect\n", 21);
				return (free_t_block(words), free_char_tab(var), NULL);
			}
			free(words[i].str);
			words[i].str = var[0];
			free(var);
		}
		else if (words[i].str[0] == 39)
		{
			words[i].spl_qu = 1;
			if (remove_qu(words, i))
				return (free_t_block(words), NULL);
		}
		else if (words[i].str[0] == '"')
		{
			words[i].dbl_qu = 1;
			if (inline_expansion(words, i, info))
				return (free_t_block(words), NULL);
			if (remove_qu(words, i))
				return (free_t_block(words), NULL);
		}
		i++;
	}
	return (words);
}

int	is_empty_var(t_block *word)
{
	size_t	i;
	size_t	tot_len;
	size_t	var_only;

	i = 0;
	tot_len = 0;
	var_only = 0;
	while (word[i].str)
	{
		tot_len += ft_strlen(word[i].str);
		var_only += word[i].var;
		i++;	
	}
	if (!tot_len && var_only == i)
		return (1);
	return (0);
}

char *get_path(char *str, t_info *info)
{
	t_block	*path;
	char	*word;

	path = expand_redirect_var(str, info);
	if (!path)
		return (NULL);
	if (is_empty_var(path))
	{
		write(2, str, ft_strlen(str));
		write(2, ": ambiguous redirect\n", 21);
		return (free_t_block(path), NULL);
	}
	word = t_block_to_str(path);
	free_t_block(path);
	if (!word)
		return (perror("Malloc error"), NULL);
	printf("path: %s\n", word);
	return (word);
}

int	handle_redirections(t_cmd *cmd, t_info *info)
{
	t_redirect	*current;
	
	current = cmd->redirections;
	while (current)
	{
		if (current->type != -2)
			current->path = get_path(current->str, info);
		if (!current->path)
			return (1);
		if (current->type < 0)
		{
			if (cmd->in)
			{
				close(cmd->in->fd);
				cmd->in->fd = -1;
			}
			current->fd = open(current->path, O_RDONLY);
			if (current->fd == -1)
			{
				write(2, current->path, ft_strlen(current->path));
				return (perror(""), 1);
			}
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
			{
				write(2, current->path, ft_strlen(current->path));
				return (perror(" "), 1);
			}
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
		info->is_child = TRUE;
		if (!handle_redirections(cmd, info))
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

t_block	*insert_back_t_block(t_block *line, size_t i, char *var)
{
	t_block	*new_line;
	size_t	j;

	new_line = (t_block *)ft_calloc((i + 2), sizeof(t_block));
	if (!new_line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_line[j] = line[j];
		j++;
	}
	new_line[j].str = var;
	return (new_line);
}

t_block	*insert_front_t_block(t_block *line, size_t i, char *var)
{
	t_block	*new_line;
	size_t	j;
	size_t	len;

	j = i + 1;
	len = 0;
	while (line[j].str)
	{
		j++;
		len++;
	}
	new_line = (t_block *)ft_calloc((len + 2), sizeof(t_block));
	if (!new_line)
		return (NULL);
	new_line[0].str = var;
	j = 1;
	len = i + 1;
	while (line[len].str)
	{
		new_line[j] = line[len];
		j++;
		len++;
	}
	return (new_line);
}

size_t	split_tab_var(t_block ***words_tab, size_t j, size_t i, char **var)
{
	size_t	x;
	size_t	y;
	size_t	nb_var;
	t_block	**new_words_tab;

	nb_var = char_tab_size(var);
	printf("nb var: %lu\n", nb_var);
	new_words_tab =	(t_block **)ft_calloc((j + nb_var + 1), sizeof(t_block *));
	if (!new_words_tab)
		return (perror("Malloc error"), free_char_tab(var), 0);
	y = 0;
	while (y < j)
	{
		new_words_tab[y] = words_tab[0][y];
		y++;
	}
	new_words_tab[y] = insert_back_t_block(words_tab[0][y], i, var[0]);
	if (!new_words_tab[y])
		return (free(new_words_tab), free_char_tab(var), 0);
	x = 1;
	while ((x + 1) < nb_var)
	{
		new_words_tab[y + x] = (t_block *)ft_calloc(2, sizeof(t_block));
		if (!new_words_tab[y + x])
		{
			while (x > 0)
			{
				free(new_words_tab[y + x]);
				x--;
			}
			return (free(new_words_tab), free_char_tab(var), 0);
		}
		new_words_tab[y + x][0].str = var[x];
		x++;
	}
	new_words_tab[y + x] = insert_front_t_block(words_tab[0][y], i, var[x]);
	if (!new_words_tab[y + x])
	{
		x--;
		while (x > 0)
		{
			free(new_words_tab[y + x]);
			x--;
		}
		return (free(new_words_tab[y]), free(new_words_tab), free_char_tab(var), 0);
	}
	free((*words_tab)[j][i].str);
	free((*words_tab)[j]);
	free(*words_tab);
	free(var);
	*words_tab = new_words_tab;
	return (nb_var);
}

void	print_t_block_tab(t_block **tab)
{
	size_t	i;
	size_t	j;

	if (!tab)
		return ;
	j = 0;
	while (tab[j])
	{
		i = 0;
		while (tab[j][i].str)
		{
			printf("%s", tab[j][i].str);
			i++;
		}
		printf("\n");
		j++;
	}
}

t_block	**add_args_word(char *str, t_info *info, int expand)
{
	size_t	word_count;
	t_block	**words_tab;
	size_t	i;
	size_t	j;
	size_t	index;
	char	**var;

	words_tab = (t_block **)ft_calloc(2, sizeof(t_block *));
	if (!words_tab)
		return (perror("Malloc error"), NULL);
	word_count = count_words_var_expansion(str);
	words_tab[0] = (t_block *)ft_calloc((word_count + 1), sizeof(t_block));
	if (!words_tab[0])
		return (perror("Malloc error"), free_t_block_tab(words_tab), NULL);
	i = 0;
	j = 0;
	index = 0;
	while (i < word_count)
	{
		words_tab[j][i].str = add_redirect_word(str, &index);
		if (!words_tab[j][i].str)
			return (free_t_block_tab(words_tab), NULL);
		i++;
	}
	i = 0;
	while (words_tab[j][i].str)
	{
		if (words_tab[j][i].str[0] == '$' && expand)
		{
			words_tab[j][i].var = 1;
			var = replace_redirect_var(words_tab[j], i, info);
			if (!var || !var[0])
			{
				free_t_block_tab(words_tab);
				if (var)
					free(var);
				return (NULL);
			}
			if (char_tab_size(var) == 1)
			{
				free(words_tab[j][i].str);
				words_tab[j][i].str = var[0];
				free(var);
			}
			else
			{
				word_count = split_tab_var(&words_tab, j, i, var);
				if (!word_count)
					return (free_t_block_tab(words_tab), NULL);
				j += (word_count - 1);
				i = 0;
			}
		}
		else if (words_tab[j][i].str[0] == 39)
		{
			words_tab[j][i].spl_qu = 1;
			if (remove_qu(words_tab[j], i))
				return (free_t_block_tab(words_tab), NULL);
		}
		else if (words_tab[j][i].str[0] == '"')
		{
			words_tab[j][i].dbl_qu = 1;
			if (expand && inline_expansion(words_tab[j], i, info))
				return (free_t_block_tab(words_tab), NULL);
			if (remove_qu(words_tab[j], i))
				return (free_t_block_tab(words_tab), NULL);
		}
		i++;
	}
	return (words_tab);
}

t_block	**add_block_to_tab(t_block **old_tab, t_block **to_add)
{
	size_t	i;
	size_t	j;
	t_block	**new_tab;

	new_tab = (t_block **)ft_calloc((t_block_tab_size(old_tab) + t_block_tab_size(to_add) + 1), sizeof(t_block *));
	if (!new_tab)
		return (free_t_block_tab(old_tab), free_t_block_tab(to_add), NULL);
	i = 0;
	while (old_tab && old_tab[i])
	{
		new_tab[i] = old_tab[i];
		i++;
	}
	j = 0;
	while (to_add && to_add[j])
	{
		new_tab[i + j] = to_add[j];
		j++;
	}
	free(old_tab);
	free(to_add);
	return (new_tab);
}

t_block	**expand_cmd_var(t_cmd *cmd, t_info *info)
{
	size_t	nb_args;
	t_block	**t_block_tab;
	t_block	**ret;

	t_block_tab = NULL;
	nb_args = 0;
	while (cmd->cmd_args[nb_args])
	{
		ret = add_args_word(cmd->cmd_args[nb_args], info, 1);
		if (!ret)
			return (perror("Malloc error"), NULL);
		t_block_tab = add_block_to_tab(t_block_tab, ret);
		if (!t_block_tab)
			return (NULL);
		nb_args++;
	}
	return (t_block_tab);
}

void	move_t_block_tab_upward(t_block **tab, size_t i, int mv)
{
	while (tab[i])
	{
		tab[i - mv] = tab[i];
		i++;
	}
	while (mv)
	{
		tab[i - mv] = NULL;
		mv--;
	}
}

int	get_final_cmd(t_cmd *cmd, t_info *info)
{
	size_t	i;
	t_block	**t_block_tab;
	char	**new_args;

	t_block_tab = expand_cmd_var(cmd, info);
	if (!t_block_tab)
		return (1);
	i = 0;
	while (t_block_tab[i])
	{
		if (is_empty_var(t_block_tab[i]))
		{
			free_t_block(t_block_tab[i]);
			t_block_tab[i] = NULL;
			move_t_block_tab_upward(t_block_tab, (i + 1), 1);
			i--;
		}
		i++;
	}
	printf("After variable expansion:\n");
	print_t_block_tab(t_block_tab);
	new_args = t_block_tab_to_char_tab(t_block_tab);
	free_t_block_tab(t_block_tab);
	if (!new_args)
		return (1);
	free_char_tab(cmd->cmd_args);
	cmd->cmd_args = new_args;
	return (0);
}

void	simple_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	if (handle_redirections(cmd, info))
	{
		info->status = 1;
		return ;
	}
	if (get_final_cmd(cmd, info))
	{
		info->status = 1;
		return ;
	}
	if (!cmd->cmd_args[0])
	{
		info->status = 0;
		return ;
	}
	if (ft_blt(cmd) == 0)
	{
		ft_blti(info, cmd);
	    return ;
	}
	ret = fork();
	if (ret == -1)
	{
		write(2, "Fork error\n", 13);
		exit (1);
	}
	if (!ret)
	{
		free_pid(info);
		if (cmd->in)
			dup2(cmd->in->fd, 0);
		if (cmd->out)
			dup2(cmd->out->fd, 1);
		ft_execute(info, cmd);
	}
	else
	{
		if (cmd->in)
		{
			close(cmd->in->fd);
			cmd->in->fd = -1;
		}
		if (cmd->out)
		{
			close(cmd->out->fd);
			cmd->out->fd = -1;
		}
		if (add_pid(info, ret))
		{
			perror("Malloc error");
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 23:08:19 by bdetune          ###   ########.fr       */
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
	int		fd[3];
	size_t	i;
	int		ret;

	i = 0;
	while (cmd->pipe[i])
	{
		if (cmd->pipe[i + 1] && pipe(fd))
				sys_call_error(info);
		ret = fork();
		if (ret == -1)
			sys_call_error(info);
		else if (!ret)
			pipe_child(info, cmd, i, fd);
		else
		{
			if (add_pid(info, ret))
				sys_call_error(info);
			fd[2] = pipe_parent(cmd, i, fd);
		}
		i++;
	}
	get_exit_status(info);
}

char	**replace_var(t_block *words, size_t i, t_info *info)
{
	char	**var_val;
	char	*var_found;

	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
	{
		var_val = (char **)ft_calloc(2, sizeof(char *));
		if (!var_val)
			return (NULL);
		if (words[i].str[1] == '?')
			var_val[0] = ft_itoa(info->status);
		else if (words[i + 1].str && (words[i + 1].str[0] == '"' || words[i + 1].str[0] == 39))
			var_val[0] = (char *)ft_calloc(1, sizeof(char));
		else
			var_val[0] = ft_strdup(words[i].str);
		if (!var_val[0])
			return (free(var_val), NULL);
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
				return (free(var_found), NULL);
			var_val[0] = var_found; 
		}
		else
		{
			var_val = ft_split_charset(var_found, "\t\n\r\v\f ");
			free(var_found);
			if (!var_val)
				return (NULL);
		}
	}
	return (var_val);
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
	t_block	**path;
	char	*word;

	path = add_args_word(str, info, 1);
	if (!path)
		return (sys_call_error(info), NULL);
	if (t_block_tab_size(path) != 1 || is_empty_var(path[0]))
	{
		write(2, "Minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": ambiguous redirect\n", 21);
		return (free_t_block_tab(path), NULL);
	}
	word = t_block_to_str(path[0]);
	free_t_block_tab(path);
	if (!word)
		return (sys_call_error(info), NULL);
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
				write(2, "Minishell: ", 11);
				write(2, current->path, ft_strlen(current->path));
				return (perror(" "), 1);
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
		return (free_char_tab(var), 0);
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
			return (execution_error(info, cmd, EXIT_FAILURE, 0), NULL);
		t_block_tab = add_block_to_tab(t_block_tab, ret);
		if (!t_block_tab)
			return (NULL);
		nb_args++;
	}
	return (t_block_tab);
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
	printf("-------------------------------------------\n");
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
	if (handle_redirections(cmd, info) || get_final_cmd(cmd, info))
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
	simple_cmd_child(info, cmd);
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

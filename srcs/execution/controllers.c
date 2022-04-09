/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/08 18:26:21 by bdetune          ###   ########.fr       */
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
			else if (ft_isdigit(str[i + 1]))
				i++;
			else
			{
				i++;
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
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

char	*find_variable(char	*var, t_info *info)
{
	char	*value;
	t_env	*current_env;
	t_var	*current_var;

	current_env = info->env;
	while (current_env)
	{
		if (!ft_strcmp(&var[1], current_env->name))
		{
			value = ft_strdup(current_env->value);
			return (value);
		}
		current_env = current_env->next;
	}
	current_var = info->local_var;
	while (current_var)
	{
		if (!ft_strcmp(&var[1], current_var->name))
		{
			value = ft_strdup(current_var->value);
			return (value);
		}
		current_var = current_var->next;
	}
	return ((char *)ft_calloc(1, sizeof(char)));
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
		var_found = find_variable(words[i].str, info);
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

int	is_whitespace(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

int	expand_dbl_qu_var(t_block *tab, size_t i, t_info *info)
{
	size_t	len;
	char	*tmp1;
	char	*tmp2;
	char	holder;
	int		j;

	j = 0;
	tmp1 = NULL;
	tmp2 = NULL;
	while (tab[i].str[j])
	{
		if (tab[i].str[j] == '$' && (ft_isalnum(tab[i].str[j + 1]) || tab[i].str[j + 1] == '?' || tab[i].str[j + 1] == '_'))
		{
			len = 0;
			if (tab[i].str[j + 1] == '?')
			{
				tmp1 = ft_itoa(info->status);
				len = 1;
			}
			else if (ft_isdigit(tab[i].str[j + 1]))
			{
				tmp1 = (char *)ft_calloc(1, sizeof(char));
				if (!tmp1)
					return (perror("Malloc error"), 1);
				len = 1;
			}
			else
			{
				j++;
				while (tab[i].str[j + len] && (ft_isalnum(tab[i].str[j + len]) || tab[i].str[j + len] == '_'))
						len++;
				holder = tab[i].str[j + len];
				tab[i].str[j + len] = '\0';
				j--;
				tmp1 = find_variable(&tab[i].str[j], info);
				tab[i].str[j + len + 1] = holder;
			}
			if (!tmp1)
				return (perror("Malloc error"), 1);
			tab[i].str[j] = '\0';
			tmp2 = ft_strjoin(tab[i].str, tmp1);
			free(tmp1);
			if (!tmp2)
				return (perror("Malloc error"), 1);
			tmp1 = ft_strjoin(tmp2, &tab[i].str[j + len + 1]);
			j = ft_strlen(tmp2) - 1;
			free(tmp2);
			if (!tmp1)
				return (perror("Malloc error"), 1);
			free(tab[i].str);
			tab[i].str = tmp1;
			tmp1 = NULL;
			tmp2 = NULL;
		}
		j++;
	}
	return (0);
}

void	free_t_block(t_block *block)
{
	size_t	i;

	i = 0;
	while (block[i].str)
	{
		free(block[i].str);
		i++;
	}
	free(block);
}

void	free_t_block_tab(t_block **block_tab)
{
	size_t	i;

	i = 0;
	while (block_tab[i])
	{
		free_t_block(block_tab[i]);
		i++;
	}
	free(block_tab);
}

void	free_char_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
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
			if (expand_dbl_qu_var(words, i, info))
				return (free_t_block(words), NULL);
			if (remove_qu(words, i))
				return (free_t_block(words), NULL);
		}
		i++;
	}
	return (words);
}

int	is_valid_redirection(t_block *path)
{
	size_t	i;
	size_t	tot_len;
	size_t	var_only;

	i = 0;
	tot_len = 0;
	var_only = 0;
	while (path[i].str)
	{
		tot_len += ft_strlen(path[i].str);
		var_only += path[i].var;
		i++;	
	}
	if (!tot_len && var_only == i)
		return (0);
	return (1);
}

char *get_path(char *str, t_info *info)
{
	t_block	*path;
	size_t	i;
	char	*word;
	char	*new_word;

	path = expand_redirect_var(str, info);
	if (!path)
		return (NULL);
	if (!is_valid_redirection(path))
	{
		write(2, str, ft_strlen(str));
		write(2, ": ambiguous redirect\n", 21);
		return (free_t_block(path), NULL);
	}
	word = ft_strdup(path[0].str);
	if (!word)
		return (free_t_block(path), perror("Malloc error"), NULL);
	i = 1;
	while (path[i].str)
	{
		new_word = ft_strjoin(word, path[i].str);
		if (!new_word)
			return (free_t_block(path), free(word), perror("Malloc error"), NULL);
		free(word);
		word = new_word;
		i++;
	}
	printf("path: %s\n", word);
	return (free_t_block(path), word);
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

size_t	char_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

size_t	t_block_tab_size(t_block **tab)
{
	size_t	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

t_block	*cpy_t_blocks(t_block *line, long long max, char *new_var)
{
	t_block	*new_line;
	size_t	line_size;
	size_t	i;

	if (max >= 0)
		line_size = (size_t)max + 1;
	else
	{
		line_size = 0;
		while (line[line_size].str)
			line_size++;
		if (max == -2)
			line_size++;
	}
	new_line = (t_block *)ft_calloc((line_size + 1), sizeof(t_block));
	if (!new_line)
		return (perror("Malloc error"), NULL);
	i = 0;
	if (max == -2)
	{
		new_line[i].str = ft_strdup(new_var);
		if (!new_line[i].str)
			return (free_t_block(new_line), NULL);
		i++;
	}
	while (line[i].str && i < (size_t)max)
	{
		new_line[line_size].str = ft_strdup(line[line_size].str);
		if (!new_line[line_size].str)
			return (free_t_block(new_line), NULL);
		i++;
	}
	if (max >= 0)
	{
		new_line[i].str = ft_strdup(new_var);
		if (!new_line[i].str)
			return (free_t_block(new_line), NULL);
	}
	return (new_line);
}

size_t	split_tab_var(t_block ***words_tab, size_t i, size_t j, char **var)
{
	size_t	x;
	size_t	y;
	size_t	nb_var;
	t_block	**new_words_tab;

	nb_var = char_tab_size(var);
	new_words_tab =	(t_block **)ft_calloc((t_block_tab_size(*words_tab) + nb_var), sizeof(t_block *));
	if (!new_words_tab)
		return (perror("Malloc error"), free_char_tab(var), 0);
	y = 0;
	while (y < j)
	{
		new_words_tab[y] = cpy_t_blocks(words_tab[0][y], -1, NULL);
		if (!new_words_tab[y])
			return (free_t_block_tab(new_words_tab), free_char_tab(var), 0);
		y++;
	}
	new_words_tab[y] = cpy_t_blocks(words_tab[0][y], (long long)i, var[0]);
	if (!new_words_tab[y])
		return (free_t_block_tab(new_words_tab), free_char_tab(var), 0);
	y++;
	x = 1;
	while ((x + 1) < nb_var)
	{
		new_words_tab[y] = cpy_t_blocks(words_tab[0][y], 0, var[x]);
		if (!new_words_tab[y])
			return (free_t_block_tab(new_words_tab), free_char_tab(var), 0);
		y++;
		x++;
	}
	new_words_tab[y] = cpy_t_blocks(words_tab[0][y], -2, var[x]);
	if (!new_words_tab[y])
		return (free_t_block_tab(new_words_tab), free_char_tab(var), 0);
	free_t_block_tab(*words_tab);
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

t_block	**add_args_word(char *str, t_info *info)
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
		if (words_tab[j][i].str[0] == '$')
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
				i = 1;
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
			if (expand_dbl_qu_var(words_tab[j], i, info))
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
		ret = add_args_word(cmd->cmd_args[nb_args], info);
		if (!ret)
			return (perror("Malloc error"), NULL);
		t_block_tab = add_block_to_tab(t_block_tab, ret);
		if (!t_block_tab)
			return (NULL);
		nb_args++;
	}
	return (t_block_tab);
}

int	get_final_cmd(t_cmd *cmd, t_info *info)
{
	t_block	**t_block_tab;

	t_block_tab = expand_cmd_var(cmd, info);
	if (!t_block_tab)
		return (1);
	printf("After variable expansion:\n");
	print_t_block_tab(t_block_tab);
	free_t_block_tab(t_block_tab);
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
		ft_execute(info, cmd->cmd_args);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/05 12:27:45 by bdetune          ###   ########.fr       */
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
		i++;
		if (str[i] == '?')
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

	printf("var sought: %s\n", var);
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

	printf("var before expansion: %s\n", words[i].str);
	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
	{
		var_val = (char **)ft_calloc(2, sizeof(char *));
		if (!var_val)
			return (perror("malloc error"), NULL);
		if (words[i].str[1] == '?')
			var_val[0] = ft_itoa(info->status);
		else if (words[i + 1].str)
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
	printf("var after expansion: %s\n", var_val[0]);
	return (var_val);
}

int	remove_qu(t_block *tab, size_t i)
{
	size_t	j;
	size_t	len;
	char	*word;

	len = ft_strlen(tab[i].str);
	tab[i].str[0] = ' ';
	tab[i].str[len - 1] = ' ';
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
			else
			{
				j++;
				while (tab[i].str[j + len] && (ft_isalnum(tab[i].str[j + len]) || tab[i].str[j + len] == '_'))
						len++;
				printf("len: %ld\n", len);
				holder = tab[i].str[j + len];
				tab[i].str[j + len] = '\0';
				j--;
				tmp1 = find_variable(&tab[i].str[j], info);
				tab[i].str[j + len + 1] = holder;
			}
			if (!tmp1)
				return (perror("Malloc error"), 1);
			printf("Variable found: %s\n", tmp1);
			tab[i].str[j] = '\0';
			tmp2 = ft_strjoin(tab[i].str, tmp1);
			printf("Variable after first join: %s\n", tmp2);
			printf("remainder: %s\n", &tab[i].str[j + len + 1]);
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
		printf("Word in redirection before: %s\n", words[i].str);
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
		printf("Word in redirection after: %s\n", words[i].str);
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

void	simple_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	if (handle_redirections(cmd, info))
	{
		info->status = 1;
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

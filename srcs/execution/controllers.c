/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/04 12:38:00 by bdetune          ###   ########.fr       */
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
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
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
		else if (ft_isalpha(str[i]) || str[i] == '_')
		{
			i++;
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

int	replace_redirect_var(char **words, size_t i, t_info *info, char *input)
{
	size_t	j;
	char	*var_val;

	if (ft_strlen(words[i]) == 1)
	{
		if (words[i + 1])
			var_val = (char *)ft_calloc(1, sizeof(char));
		else
			var_val = ft_strdup(words[i]);
	}
	else if (words[i][1] == '?')
		var_val = ft_itoa(info->status);
	else
		var_val = find_variable(words[i], info);
	if (!var_val)
			return (perror("Malloc error"), 1);
	j = 0;
	while (var_val[j])
	{
		if ((var_val[j] >= '\t' && var_val[j] <= '\r') || var_val[j] == ' ')
		{
			free(var_val);
			write(2, input, ft_strlen(input));
			write(2, ": ambiguous redirect\n", 21);
			return (1);
		}
		j++;
	}
	free(words[i]);
	words[i] = var_val;
	return (0);
}

int	remove_qu(char **tab, size_t i)
{
	size_t	j;
	size_t	len;
	char	*word;

	len = ft_strlen(tab[i]);
	tab[i][0] = ' ';
	tab[i][len - 1] = ' ';
	word = (char *)ft_calloc((len - 1), sizeof(char));
	if (!word)
		return (perror("Malloc error"), 1);
	j = 0;
	while (j < (len - 2))
	{
		word[j] = tab[i][j + 1];
		j++;
	}
	free(tab[i]);
	tab[i] = word;
	return (0);
}

int	is_whitespace(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

int	expand_dbl_qu_var(char **tab, size_t i, t_info *info)
{
	size_t	len;
	char	*tmp1;
	char	*tmp2;
	char	holder;
	size_t	j;

	j = 0;
	tmp1 = NULL;
	tmp2 = NULL;
	while (tab[i][j])
	{
		if (tab[i][j] == '$' && !(tab[i][j + 1] == 39 || tab[i][j + 1] == '"'
				|| tab[i][j + 1] == '$' || tab[i][j + 1] == '/' || tab[i][j + 1] == '\\' || is_whitespace(tab[i][j + 1])))
		{
			len = 0;
			if (tab[i][j + 1] == '?')
			{
				tmp1 = ft_itoa(info->status);
				len = 1;
			}
			else
			{
				j++;
				while (tab[i][j + len] && !(tab[i][j + len] == 39
					|| tab[i][j + len] == '"' || tab[i][j + len] == '$' || tab[i][j + len] == '?'
					|| tab[i][j + len] == '/' || tab[i][j + len] == '\\' || is_whitespace(tab[i][j + len])))
						len++;
				holder = tab[i][j + len];
				tab[i][j + len] = '\0';
				j--;
				tmp1 = find_variable(&tab[i][j], info);
				tab[i][j + len + 1] = holder;
			}
			if (!tmp1)
				return (perror("Malloc error"), 1);
			printf("Variable found: %s\n", tmp1);
			tab[i][j] = '\0';
			tmp2 = ft_strjoin(tab[i], tmp1);
			printf("Variable after first join: %s\n", tmp2);
			printf("remainder: %s\n", &tab[i][j + len + 1]);
			free(tmp1);
			if (!tmp2)
				return (perror("Malloc error"), 1);
			tmp1 = ft_strjoin(tmp2, &tab[i][j + len + 1]);
			j = ft_strlen(tmp2) - 1;
			free(tmp2);
			if (!tmp1)
				return (perror("Malloc error"), 1);
			free(tab[i]);
			tab[i] = tmp1;
			tmp1 = NULL;
			tmp2 = NULL;
		}
		j++;
	}
	if (remove_qu(tab, i))
		return (1);
	return (0);
}

t_block	*expand_redirect_var(char *str, t_info *info)
{
	size_t	word_count;
	t_block	*words;
	size_t	i;
	size_t	index;

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
		{
			while (i--)
				free(words[i].str);
			return (free(words), NULL);
		}
		i++;
	}
	i = 0;
	while (words[i])
	{
		printf("Word in redirection before: %s\n", words[i]);
		if (words[i][0] == '$')
		{
			if (replace_redirect_var(words, i, info, str))
			{
				i = 0;
				while (words[i])
				{
					free(words[i]);
					i++;
				}
				return (free(words), NULL);
			}
		}
		else if (words[i][0] == 39)
		{
			if (remove_qu(words, i))
				return (NULL);
		}
		else if (words[i][0] == '"')
		{
			if (expand_dbl_qu_var(words, i, info))
				return (NULL);
		}
		printf("Word in redirection after: %s\n", words[i]);
		i++;
	}
	return (words);
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

char *get_path(char *str, t_info *info)
{
	char	**path;
	size_t	i;
	char	*word;
	char	*new_word;

	path = expand_redirect_var(str, info);
	if (!path)
		return (NULL);
	word = ft_strdup(path[0]);
	if (!word)
		return (free_char_tab(path), perror("Malloc error"), NULL);
	i = 1;
	while (path[i])
	{
		new_word = ft_strjoin(word, path[i]);
		if (!new_word)
			return (free_char_tab(path), free(word), perror("Malloc error"), NULL);
		free(word);
		word = new_word;
		i++;
	}
	printf("path: %s\n", word);
	if (!ft_strlen(word))
	{
		write(2, str, ft_strlen(str));
		write(2, ": ambiguous redirect\n", 21);
		free(word);
		word = NULL;
	}
	return (free_char_tab(path), word);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/25 15:21:39 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	next_parenth(char *str, size_t *i)
{
	int	dbl_qu;
	int	spl_qu;

	dbl_qu = 0;
	spl_qu = 0;
	while (str[*i])
	{
		if (str[*i] == dbl_qu && !spl_qu)
			dbl_qu ^= 1;
		else if (str[*i] == spl_qu && !dbl_qu)
			spl_qu ^= 1;
		if (str[*i] == ')' && !spl_qu && !dbl_qu)
			break ;
		*i += 1;
	}
}

int	has_parenthesis(char *str)
{
	size_t	i;
	int		dbl_qu;
	int		spl_qu;

	dbl_qu = 0;
	spl_qu = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == dbl_qu && !spl_qu)
			dbl_qu ^= 1;
		else if (str[i] == spl_qu && !dbl_qu)
			spl_qu ^= 1;
		if (str[i] == '(' && !spl_qu && !dbl_qu)
		{
			if (i > 0 && str[i - 1] == '=')
				next_parenth(str, &i);
			else
				return (1);
		}
		i++;
	}
	return (0);
}*/

char	*ft_del_spaces(char *str)
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
		return (NULL);
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
	new_redirect->fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (new_redirect->fd == -1)
		return (perror("Error creating here-document"), 1);
	new_redirect->path = path;
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

int	add_redirect(char *str, t_cmd *cmd, int redirect)
{
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!new_redirect)
		return (1);
	new_redirect->type = redirect;
	new_redirect->fd = -1;
	new_redirect->var_expansion = 1;
	new_redirect->path = NULL;
	new_redirect->next = cmd->redirections;
	while (!(*str == '>' || *str == '<'))
		str++;
	new_redirect->str = ft_trim(&str[ft_abs(new_redirect->type)]);
	if (!new_redirect->str)
		return (free(new_redirect), 1);
	if (new_redirect->type == -2)
	{
		if (save_heredoc(new_redirect))
			return (free(new_redirect->str), free(new_redirect), 1);
	}
	cmd->redirections = new_redirect;
	return (0);
}

int	save_redirect(char *str, size_t i, t_cmd *cmd)
{
	int		redirect;

	if (str[i] == ')')
	{
		str[i] = '\0';
		return (0);
	}
	if (str[i] == '<' || str[i] == '>' || str[i] == '&' || str[i] == '|')
		return (1);
	while (str[i] > 32 && str[i] < 127 && str[i] != '<' && str[i] != '>'
		&& str[i] != ')' && str[i] != '(' && str[i] != '&' && str[i] != '|')
		i--;
	if (str[i] == '&' || str[i] == '|')
		return (1);
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i--;
	if (!(str[i] == '<' || str[i] == '>'))
		return (1);
	if (str[i] == '<')
	{
		redirect = -1;
		if (str[i - 1] == '<')
		{
			redirect = -2;
			i--;
		}
		i--;
	}
	else if (str[i] == '>')
	{
		redirect = 1;
		if (str[i - 1] == '>')
		{
			redirect = 2;
			i--;
		}
		i--;
	}
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i--;
	if (add_redirect(&str[i + 1], cmd, redirect))
		return (1);
	str[i + 1] = '\0';
	return (save_redirect(str, i, cmd));
}

int	fork_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*new_cmd;

	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (cmd->cmd[i] != '(')
		return (0);
	cmd->cmd[i] = ' ';
	while (cmd->cmd[i])
		i++;
	i--;
	if (save_redirect(cmd->cmd, i, cmd))
		return (write(2, "Parsing error\n", 14), 2);
	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (2);
	cmd->fork = new_cmd;
	new_cmd->cmd = ft_trim(cmd->cmd);
	if (!new_cmd->cmd)
		return (2);
	if (parse_cmd(new_cmd))
		return (2);
	return (1);
}

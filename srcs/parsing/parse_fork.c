/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/29 14:19:44 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_redirect(char *str, t_cmd *cmd, int redirect)
{
	t_redirect	*current;
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!new_redirect)
		return (perror("Malloc error"), 1);
	new_redirect->type = redirect;
	new_redirect->fd = -1;
	if (!cmd->redirections)
		cmd->redirections = new_redirect;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = new_redirect;
	}
	new_redirect->str = ft_trim(str);
	if (!new_redirect->str)
		return (perror("Malloc error"), 1);
	if (new_redirect->type == -2)
	{
		if (save_heredoc(new_redirect))
			return (1);
	}
	return (0);
}

int	save_redirect(char *str, int i, t_cmd *cmd)
{
	int		j;
	int		spl_qu;
	int		dbl_qu;
	int		redirect;
	char	tmp;

	skip_whitespaces(str, &i);
	if (str[i] == '\0')
		return (0);
	if (!(str[i] == '<' || str[i] == '>'))
	{
		write(2, "Syntax error near unexpected token `", 36);
		write(2, &str[i], 1);
		return (write(2, "'\n", 2), 1);
	}
	if (str[i] == '<')
	{
		redirect = -1;
		if (str[i + 1] == '<')
		{
			redirect = -2;
			i++;
		}
		else if (str[i + 1] == '>')
			return (write(2, "Syntax error near unexpected token `>'\n", 39), 1);
		i++;
	}
	else if (str[i] == '>')
	{
		redirect = 1;
		if (str[i + 1] == '>')
		{
			redirect = 2;
			i++;
		}
		else if (str[i + 1] == '<')
			return (write(2, "Syntax error near unexpected token `<'\n", 39), 1);
		i++;
	}
	skip_whitespaces(str, &i);
	j = i;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i] > 32 && str[i] < 127)
	{
		if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[i] == '<' || str[i] == '>'
				|| str[i] == ')' || str[i] == '(' || str[i] == '&'
				|| str[i] == '|'))
				break ;
		i++;
	}
	if (str[i] == ')' || str[i] == '(' || str[i] == '&' || str[i] == '|')
		return (write(2, "Syntax error near unexpected token\n", 35), 1);
	tmp = str[i];
	str[i] = '\0';
	if (!ft_strlen(&str[j]))
	{
		write(2, "Syntax error near unexpected token `", 36);
		write(2, &tmp, 1);
		return (write(2, "'\n", 2), 1);
	}
	if (add_redirect(&str[j], cmd, redirect))
		return (1);
	str[i] = tmp;
	return (save_redirect(str, i, cmd));
}

int	skip_closing_parenth(char *str, int *i)
{
	int	dbl_qu;
	int	spl_qu;
	int	parenth;

	dbl_qu = 0;
	spl_qu = 0;
	parenth = 0;
	while (str[*i])
	{
		if (str[*i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (str[*i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (!spl_qu && !dbl_qu && str[*i] == '(')
			parenth++;
		else if (!spl_qu && !dbl_qu && str[*i] == ')')
		{
			parenth--;
			if (parenth == 0)
				break ;
		}
		*i += 1;
	}
	if (str[*i] != ')')
		return (write(2, "Syntax error near unexpected token `('\n", 39), 1);
	return (0);
}

int	fork_cmd(t_cmd *cmd)
{
	int		i;
	int		j;
	t_cmd	*new_cmd;

	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (cmd->cmd[i] != '(')
		return (0);
	j = i;
	if (skip_closing_parenth(cmd->cmd, &i))
		return (2);
	cmd->cmd[i] = '\0';
	cmd->cmd[j] = ' ';
	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (perror("Malloc error"), 2);
	cmd->fork = new_cmd;
	new_cmd->cmd = ft_trim(cmd->cmd);
	if (!new_cmd->cmd)
		return (write(2, "Syntax error near unexpected token `)'\n", 39), 2);
	if (parse_cmd(new_cmd))
		return (2);
	i++;
	if (save_redirect(cmd->cmd, i, cmd))
		return (2);
	return (1);
}

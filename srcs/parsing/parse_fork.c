/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 16:03:34 by bdetune          ###   ########.fr       */
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
	while (str[i] > 32 && str[i] < 127)
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
				i++;
		}
		else if (str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
		}
		else if (str[i] == '<' || str[i] == '>' || str[i] == ')'
				|| str[i] == '(' || str[i] == '&' || str[i] == '|')
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

static void	skip_closing_parenth(char *str, int *i)
{
	int			j;
	t_tokens	toks;

	j = *i;
	init_tokens(&toks);
	while (str[*i])
	{
		if (save_token(str[*i], &toks) && !has_tokens(toks) && str[*i] == ')')
			break ;
		*i += 1;
	}
	str[j] = ' ';
	str[*i] = '\0';
}

int	fork_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*new_cmd;

	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (cmd->cmd[i] != '(')
		return (0);
	skip_closing_parenth(cmd->cmd, &i);
	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (perror("Malloc error"), 2);
	cmd->fork = new_cmd;
	new_cmd->cmd = ft_trim(cmd->cmd);
	if (!new_cmd->cmd)
		return (parsing_error(PARENTH, NULL, NULL), 2);
	save_delim(new_cmd, &cmd->cmd[i], PARENTH, PARENTH);
	if (parse_cmd(new_cmd))
		return (2);
	i++;
	if (save_redirect(cmd->cmd, i, cmd))
		return (2);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/17 14:44:18 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (1);
		i++;
	}
	return (0);
}

int	add_redirect(char *str, t_cmd *cmd, int redirect)
{
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!new_redirect)
		return (1);
	if (redirect < 0)
	{
		new_redirect->type = -redirect;
		new_redirect->next = cmd->in;
		cmd->in = new_redirect;
	}
	else
	{
		new_redirect->type = redirect;
		new_redirect->next = cmd->out;
		cmd->out = new_redirect;
	}
	while (!(*str == '>' || *str == '<'))
		str++;
	new_redirect->str = ft_trim(&str[new_redirect->type]);
	if (!new_redirect->str)
		return (free(new_redirect), 1);
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

	if (!has_parenthesis(cmd->cmd))
		return (0);
	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (2);
	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (cmd->cmd[i] != '(')
		return (free(new_cmd), write(2, "Parsing error\n", 14), 2);
	cmd->cmd[i] = ' ';
	while (cmd->cmd[i])
		i++;
	i--;
	if (save_redirect(cmd->cmd, i, cmd))
		return (free(new_cmd), write(2, "Parsing error\n", 14), 2);
	cmd->fork = new_cmd;
	new_cmd->cmd = ft_trim(cmd->cmd);
	if (!new_cmd->cmd)
		return (2);
	if (parse_cmd(new_cmd))
		return (2);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 21:03:24 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid(char *str)
{
	int	i;
	int	spl_qu;
	int	dbl_qu;

	while (str[i] && ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' '))
		i++;
	if (str[i] == '\0')
		return (0);
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[i] == '&' || str[i] == '|'
				|| str[i] == '(' || str[i] == ')'))
			return (0);
		i++;
	}
	return (1);
}

static int	add_redirect(t_cmd *cmd, int i, int j, int redirect)
{
	//asdd redirect
	return (0);
}

static int	save_redirect(t_cmd *cmd, char *str, int i)
{
	int	redirect;
	int	j;

	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
			break;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	if (str[i] == '>')
	{
		redirect = 1;
		if (str[i + 1] == '>')
			redirect = 2;
	}
	else if (str[i] == '<')
	{
		redirect = -1;
		if (str[i + 1] == '<')
			redirect = -2
	}
	j = i;
	skip_whitespaces(str, &j);
	if (str[j] == '\0')
		return (1);
	while (str[j] && str[j] > 32  && str[j] < 127)
		j++;
	if (add_redirect(cmd, i, j, redirect))
		return (1);
	while (i < j)
		str[i++] = ' ';
	return (cmd, str, i);
}

int	parse_simple_cmd(t_cmd *cmd)
{
	int	i;

	if (!is_valid(cmd->cmd))
		return (write (2, "Parsing error\n", 14), 1);
	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (save_redirect(cmd, cmd->cmd, i))
		return (1);
	cmd->cmd_name = cmd->cmd;
	return (0);
}

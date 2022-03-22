/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/22 14:08:32 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid(char *str, int i)
{
	int	spl_qu;
	int	dbl_qu;

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
		else if (!dbl_qu && !spl_qu && (str[i] == '|'
				|| str[i] == '(' || str[i] == ')'))
			return (0);
		i++;
	}
	return (1);
}

static int	add_redirect(t_cmd *cmd, int i, int redirect)
{
	t_redirect	*current;
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!new_redirect)
		return (1);
	new_redirect->next = NULL;
	if (redirect < 0)
	{
		new_redirect->type = -redirect;
		if (!cmd->in)
			cmd->in = new_redirect;
		else
		{
			current = cmd->in;
			while (current->next)
				current = current->next;
			current->next = new_redirect;
		}
	}
	else
	{
		new_redirect->type = redirect;
		if (!cmd->out)
			cmd->out = new_redirect;
		else
		{
			current = cmd->out;
			while (current->next)
				current = current->next;
			current->next = new_redirect;
		}
	}
	new_redirect->str = ft_trim(&cmd->cmd[i + new_redirect->type]);
	if (!new_redirect->str)
		return (1);
	return (0);
}

static int	save_redirect(t_cmd *cmd, char *str, int i)
{
	char		temp;
	int			end;
	int			redirect;
	int			j;
	t_tokens	toks;

	init_tokens(&toks);
	while (str[i])
	{
		save_token(str[i], &toks);
		if (!has_tokens(toks) && (str[i] == '>' || str[i] == '<'))
			break ;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	j = i;
	if (str[i] == '>' && ++j)
	{
		redirect = 1;
		if (str[i + 1] == '>' && ++j)
			redirect = 2;
	}
	else if (str[i] == '<' && ++j)
	{
		redirect = -1;
		if (str[i + 1] == '<' && ++j)
			redirect = -2;
	}
	skip_whitespaces(str, &j);
	if (str[j] == '\0')
		return (1);
	while (str[j] && str[j] > 32 && str[j] < 127
		&& !(str[j] == '<' || str[j] =='>'))
		j++;
	if (str[j] != '\0')
	{
		temp = str[j];
		str[j] = '\0';
		end = 0;
	}
	else
		end = 1;
	if (add_redirect(cmd, i, redirect))
		return (1);
	while (str[i])
		str[i++] = ' ';
	if (end)
		return (0);
	else
	{
		str[i] = temp;
		return (save_redirect(cmd, cmd->cmd, i));
	}
}

int	parse_simple_cmd(t_cmd *cmd)
{
	char	*trimmed;
	int		i;

	i = 0;
	skip_whitespaces(cmd->cmd, &i);
	if (!is_valid(cmd->cmd, i))
		return (write (2, "Parsing error\n", 14), 1);
	if (save_redirect(cmd, cmd->cmd, i))
		return (1);
	trimmed = ft_trim(cmd->cmd);
	free(cmd->cmd);
	cmd->cmd = trimmed;
	if (parse_args(cmd))
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/29 15:36:56 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
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
				|| (str[i] == '&' && str[i + 1] == '&') || str[i] == ')'))
			break ;
		else if (!dbl_qu && !spl_qu && str[i] == '(')
		{
			
		}
		i++;
	}
	if (str[i])
	{
		write(2, "Syntax error near token `", 25);
		if (str[i] == '&')
			write(2, "&&", 2);
		else
			write(2, &str[i], 1);
		write(2, "'\n", 2);
		return (1);
	}
	return (1);
}*/

static int	add_redirect(t_cmd *cmd, int i, int redirect)
{
	t_redirect	*current;
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!new_redirect)
		return (1);
	new_redirect->type = redirect;
	new_redirect->fd = -1;
	new_redirect->var_expansion = 1;
	if (!cmd->redirections)
		cmd->redirections = new_redirect;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = new_redirect;
	}
	new_redirect->str = ft_trim(&cmd->cmd[i + ft_abs(new_redirect->type)]);
	if (!new_redirect->str)
		return (1);
	if (new_redirect->type == -2)
	{
		if(save_heredoc(new_redirect))
			return (1);
	}
	return (0);
}

static int	save_redirect(t_cmd *cmd, char *str, int i, int has_cmd)
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

static int	check_cmd(t_cmd *cmd)
{
	size_t	i;
	int		has_cmd;

	has_cmd = 0;
	i = 0;
	while (cmd->cmd_args[i])
	{
		if (!has_cmd && is_assignation(cmd->cmd_args[i]))
		{
			if (!is_valid_assignation(cmd->cmd_args[i]))
				return (1);
		}
		else if (cmd->cmd_args[0] == '>' || cmd->cmd_args[0] == '<')
		{
			if (handle_redirections(cmd, &i))
				return (1);
		}
		else if (has_illegal_char(cmd->cmd_args[i]))
			return (1);
		else
			has_cmd = 1;
		i++;
	}
	return (0);
}

int	parse_simple_cmd(t_cmd *cmd)
{
	char	*trimmed;
	int		i;

//	i = 0;
//	skip_whitespaces(cmd->cmd, &i);
//	if (!cmd->cmd[i])
//		return (1);
//	check_cmd(cmd, cmd->cmd, i, 0);
//	if (!is_valid(cmd->cmd, i))
//		return (1);
//	if (save_redirect(cmd, cmd->cmd, i))
//		return (1);
	trimmed = ft_trim(cmd->cmd);
	if (!trimmed)
		return (perror("Malloc error"), 1);
	free(cmd->cmd);
	cmd->cmd = trimmed;
	if (parse_args(cmd))
		return (1);
	if(check_cmd(cmd))
		return (1);
	return (0);
}

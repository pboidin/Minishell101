/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 17:50:17 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
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
	new_redirect->str = ft_strdup(str);
	if (!new_redirect->str)
		return (perror("Malloc error"), 1);
	if (new_redirect->type == -2)
	{
		if (save_heredoc(new_redirect))
			return (1);
	}
	return (0);
}

int	get_redirect_type(char *str)
{
	int	redirect;

	redirect = 0;
	if (ft_strlen(str) == 2)
	{
		if (str[0] == '<')
			redirect = -2;
		else
			redirect = 2;
	}
	else
	{
		if (str[0] == '<')
			redirect = -1;
		else
			redirect = 1;
	}
	return (redirect);
}

int	is_valid_redirect(char *str)
{
	size_t	i;
	int		spl_qu;
	int		dbl_qu;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[i] == '(' || str[i] == ')'
				|| str[i] == '|' || (str[i] == '&' && str[i + 1] == '&')
				|| str[i] == '<' || str[i] =='>'))
			return (0);
		i++;
	}
	return (1);
}
*/
static int	save_redirect(t_cmd *cmd, char *str)
{
	int	i;
	int	redirect;

	i = 0;
	skip_whitespaces(str, &i);
	if (!str[i])
		return (0);
	if (!parse_args(cmd, str))
		return (1);
	i = 0;
	while (cmd->cmd_args[i])
	{
		if (!(cmd->cmd_args[i][0] == '<' || cmd->cmd_args[i][0] == '>'))
			return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
		else
			redirect = get_redirect_type(cmd->cmd_args[i]);
		i++;
		if (!cmd->cmd_args[i])
			return (parsing_error(cmd->next_delim, NULL, NULL), 1);
		if (!is_valid_arg(cmd->cmd_args[i]))
			return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
		if (add_redirect(cmd->cmd_args[i], cmd, redirect))
			return (1);
		i++;
	}
	return (0);
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
	if (save_redirect(cmd, &cmd->cmd[i]))
		return (2);
	return (1);
}

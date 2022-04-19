/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:40:49 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 13:01:57 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	i = -1;
	while (cmd->cmd_args[++i] && g_signal <= 0)
	{
		if (!(cmd->cmd_args[i][0] == '<' || cmd->cmd_args[i][0] == '>'))
			return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
		redirect = get_redirect_type(cmd->cmd_args[i]);
		i++;
		if (!cmd->cmd_args[i])
			return (parsing_error(cmd->next_delim, NULL, NULL), 1);
		if (!is_valid_arg(cmd->cmd_args[i]))
			return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
		if (add_redirect(cmd->cmd_args[i], cmd, redirect))
			return (1);
	}
	return (0);
}

void	skip_closing_parenth(char *str, int *i)
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

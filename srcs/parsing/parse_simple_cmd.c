/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 13:08:43 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_redirections(t_cmd *cmd, int *i)
{
	int		redirect;

	redirect = get_redirect_type(cmd->cmd_args[*i]);
	if (!cmd->cmd_args[*i + 1])
		return (parsing_error(cmd->next_delim, NULL, NULL), 1);
	if (!is_valid_arg(cmd->cmd_args[*i + 1]))
		return (parsing_error(-1, cmd->cmd_args[*i + 1], NULL), 1);
	if (add_redirect(cmd->cmd_args[*i + 1], cmd, redirect))
		return (1);
	free(cmd->cmd_args[*i]);
	free(cmd->cmd_args[*i + 1]);
	cmd->cmd_args[*i] = NULL;
	cmd->cmd_args[*i + 1] = NULL;
	move_upward(cmd, (*i + 2), 2);
	*i -= 1;
	return (0);
}

static int	check_assignations_and_redirections(t_cmd *cmd, int *i)
{
	if (cmd->cmd_args[*i][0] == '>' || cmd->cmd_args[*i][0] == '<')
	{
		if (parse_redirections(cmd, i))
			return (1);
	}
	else
	{
		if (!is_valid_assignation(cmd->cmd_args[*i]))
			return (1);
	}
	return (0);
}

static int	check_cmd(t_cmd *cmd)
{
	int		i;
	int		has_cmd;
	int		is_export;

	has_cmd = 0;
	i = -1;
	is_export = 0;
	while (cmd->cmd_args[++i] && g_signal == 0)
	{
		if (((!has_cmd || is_export) && is_assignation(cmd->cmd_args[i]))
			|| (cmd->cmd_args[i][0] == '>' || cmd->cmd_args[i][0] == '<'))
		{
			if (check_assignations_and_redirections(cmd, &i))
				return (1);
		}
		else
		{
			if (!is_valid_arg(cmd->cmd_args[i]))
				return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
			if (!has_cmd && !strcmp("export", cmd->cmd_args[i]))
				is_export = clean_previous_args(cmd, &i);
			has_cmd = 1;
		}
	}
	return (0);
}

int	parse_simple_cmd(t_cmd *cmd)
{
	int	nb_args;

	nb_args = parse_args(cmd, cmd->cmd);
	if (!nb_args)
		return (1);
	if (check_cmd(cmd))
		return (1);
	return (0);
}

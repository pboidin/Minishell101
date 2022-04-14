/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:47:31 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/14 22:06:45 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sys_call_error(t_info *info)
{
	perror("Error");
	free_info(info);
	exit(EXIT_FAILURE);
}

void	execution_error(t_info *info, t_cmd *cmd, int exit_code, int absolute)
{
	if (exit_code == 127)
	{
		if (absolute)
		{
			write(2, "Minishell: ", 11);
			write(2, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
			write(2, ": No such file or directory\n", 28);
		}
		else
		{
			write(2, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
			write(2, ": command not found\n", 20);
		}
	}
	else if (exit_code == 126)
	{
		write(2, "Minishell: ", 11);
		write(2, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
		write(2, ": Permission denied\n", 20);
	}
	else
		perror("Error");
	free_info(info);
	exit(exit_code);
}

void	parsing_error(int delim, char *str, t_tokens *toks)
{
	write(2, "Minishell: syntax error near unexpected token `", 47);
	if (delim >= 0)
	{
		if (delim == 0)
			write(2, "newline", 7);
		else if (delim == PIPE)
			write(2, "|", 1);
		else if (delim == OR)
			write(2, "||", 2);
		else if (delim == AND)
			write(2, "&&", 2);
		else if (delim == PARENTH)
			write(2, ")", 1);
	}
	else if (str)
		write(2, str, ft_strlen(str));
	else if (toks && (toks->dbl_qu || toks->spl_qu || toks->par > 0))
		write(2, "newline", 7);
	else if (toks && toks->par < 0)
		write(2, ")", 1);
	write(2, "'\n", 2);
}

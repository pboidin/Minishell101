/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 13:07:35 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/17 13:22:56 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char *str)
{
	t_tokens	toks;
	int			i;
	int			nb_command;

	i = 0;
	nb_command = 0;
	init_tokens(&toks);
	while (str[i])
	{
		skip_whitespaces(str, &i);
		nb_command++;
		while (str[i])
		{
			save_token(str[i], &toks);
			if (!has_tokens(toks) && ((str[i] >= '\t' && str[i] <= '\r')
					|| str[i] == ' '))
				break;
			i++;
		}
	}
	return (nb_command);
}

static char	**split_args(char *str, int nb_args)
{
	int		index;
	int		i;
	char	**tab;

	tab = (char **)malloc(sizeof(char *) * (nb_args + 1));
	if (!tab)
		return (NULL);
	i = 0;
	index = 0;
	while (i < nb_args)
	{

		i++;
	}

}

int	parse_args(t_cmd *cmd)
{
	int i;
	int	nb_args;

	nb_args = count_args(cmd->cmd);
	if (nb_args == 1)
	{
		cmd->cmd_name = ft_trim(cmd->cmd);
		if (!cmd->cmd_name)
			return (1);
		cmd->cmd_args = (char **)malloc(sizeof(char *) * 2);
		if (!cmd->cmd_args)
			return (1);
		cmd->cmd_args[0] = ft_trim(cmd->cmd);
		if (!cmd->cmd_args[0])
			return (1);
		cmd->cmd_args[1] = NULL;
	}
	else
	{
		cmd->cmd_args = split_args(cmd->cmd, nb_args);
		if (!cmd->cmd_args)
			return (1);
		cmd->cmd_name = ft_trim(cmd->cmd_args[0]);
		if (!cmd->cmd_name)
			return (1);
	}
	return (0);
}

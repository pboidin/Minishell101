/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 13:07:35 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/18 17:40:20 by bdetune          ###   ########.fr       */
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
		if (str[i])
			nb_command++;
		while (str[i])
		{
			save_token(str[i], &toks);
			if (!has_tokens(toks) && ((str[i] >= '\t' && str[i] <= '\r')
					|| str[i] == ' '))
				break ;
			i++;
		}
	}
	return (nb_command);
}

static char	*save_arg(char *str, int *index)
{
	char		*arg;
	t_tokens	toks;
	int			i;

	init_tokens(&toks);
	skip_whitespaces(str, index);
	i = *index;
	while (str[i])
	{
		save_token(str[i], &toks);
		if (!has_tokens(toks) && ((str[i] >= '\t' && str[i] <= '\r')
				|| str[i] == ' '))
			break ;
		i++;
	}
	str[i] = '\0';
	arg = ft_trim(&str[*index]);
	if (!arg)
		return (NULL);
	*index = i + 1;
	return (arg);
}

static char	**split_args(char *str, int nb_args)
{
	int			index;
	int			i;
	char		**tab;

	tab = (char **)malloc(sizeof(char *) * (nb_args + 1));
	if (!tab)
		return (NULL);
	i = 0;
	index = 0;
	while (i < nb_args)
	{
		tab[i] = save_arg(str, &index);
		if (!tab[i])
			return (NULL);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

int	parse_args(t_cmd *cmd)
{
	int	nb_args;

	if (!cmd->cmd)
	{
		cmd->cmd_args = (char **)malloc(sizeof(char *));
		if (!cmd->cmd_args)
			return (1);
		cmd->cmd_args[0] = NULL;
		return (0);
	}
	nb_args = count_args(cmd->cmd);
	cmd->cmd_args = split_args(cmd->cmd, nb_args);
	if (!cmd->cmd_args)
		return (1);
	return (0);
}

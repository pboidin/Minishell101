/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 13:07:35 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 16:56:39 by bdetune          ###   ########.fr       */
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
		if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
			i += 2;
		else if (str[i] == '>' || str[i] == '<')
			i++;
		else
		{
			while (str[i])
			{
				save_token(str[i], &toks);
				if (!has_tokens(toks) && (((str[i] >= '\t' && str[i] <= '\r')
						|| str[i] == ' ') || str[i] == '<' || str[i] == '>'))
					break ;
				i++;
			}
		}
	}
	return (nb_command);
}

static char	*save_arg(char *str, int *index)
{
	char		tmp;
	char		*arg;
	t_tokens	toks;
	int			i;

	init_tokens(&toks);
	skip_whitespaces(str, index);
	i = *index;
	if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
		i += 2;
	else if (str[i] == '>' || str[i] == '<')
		i++;
	else
	{
		while (str[i])
		{
			save_token(str[i], &toks);
			if (!has_tokens(toks) && (((str[i] >= '\t' && str[i] <= '\r')
				|| str[i] == ' ') || str[i] == '<' || str[i] == '>'))
				break ;
			i++;
		}
	}
	tmp = str[i];
	str[i] = '\0';
	arg = ft_trim(&str[*index]);
	if (!arg)
		return (perror("Malloc error"), NULL);
	str[i] = tmp;
	*index = i;
	return (arg);
}

static char	**split_args(char *str, int nb_args)
{
	int			index;
	int			i;
	char		**tab;

	tab = (char **)ft_calloc((nb_args + 1), sizeof(char *));
	if (!tab)
		return (perror("Malloc error"), NULL);
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

int	parse_args(t_cmd *cmd, char *str)
{
	int	nb_args;

	nb_args = count_args(str);
	if (nb_args == 0)
		return (write(2, "Empty command\n", 14), 0);
	cmd->cmd_args = split_args(str, nb_args);
	if (!cmd->cmd_args)
		return (0);
	return (nb_args);
}

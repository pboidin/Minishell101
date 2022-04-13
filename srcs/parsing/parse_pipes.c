/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:06:08 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 12:20:57 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*add_pipe(char *cmd, int *i)
{
	char		tmp;
	t_tokens	toks;
	int			j;
	t_cmd		*new_cmd;

	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (perror("Malloc error"), NULL);
	init_tokens(&toks);
	j = 0;
	while (cmd[*i + j])
	{
		if (!save_token(cmd[*i + j], &toks) && cmd[*i + j] == '|'
			&& !has_tokens(toks))
			break ;
		j++;
	}
	tmp = cmd[*i + j];
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	if (!new_cmd->cmd)
		return (free(new_cmd), parsing_error(NULL, tmp), NULL);
	*i += j + 1;
	return (new_cmd);
}

static long long	count_pipes(char *cmd)
{
	t_tokens	toks;
	size_t		i;
	long long	nb_cmds;

	init_tokens(&toks);
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		if (!save_token(cmd[i], &toks) && cmd[i] == '|' && !has_tokens(toks))
			nb_cmds++;
		i++;
	}
	nb_cmds++;
	return (nb_cmds);
}

int	parse_pipe(t_cmd *cmd)
{
	int			index;
	long long	i;
	long long	j;

	if (!cmd || !cmd->cmd)
		return (1);
	i = count_pipes(cmd->cmd);
	if (i == 1)
		return (0);
	cmd->pipe = (t_cmd **)ft_calloc((i + 1), sizeof(t_cmd *));
	if (!cmd->pipe)
		return (1);
	j = 0;
	index = 0;
	while (j < i)
	{
		cmd->pipe[j] = add_pipe(cmd->cmd, &index);
		if (!cmd->pipe[j])
			return (1);
		cmd->pipe[j]->prev_delim = 1;
		cmd->pipe[j]->next_delim = 1;
		if (parse_cmd(cmd->pipe[j]))
			return (1);
		j++;
	}
	cmd->pipe[0]->prev_delim = cmd->prev_delim;
	cmd->pipe[i - 1]->next_delim = cmd->nex_delim;
	return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:06:08 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 14:45:09 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*add_pipe(char *cmd, size_t *i, int prev_delim, t_cmd *father)
{
	t_tokens	toks;
	size_t		j;
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
	save_delim(new_cmd, &cmd[*i + j], prev_delim, father->next_delim);
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	if (!new_cmd->cmd)
		return (free(new_cmd), parsing_error(father->next_delim, NULL, NULL), \
		NULL);
	*i += j + 1;
	return (new_cmd);
}

static size_t	count_pipes(char *cmd)
{
	t_tokens	toks;
	size_t		i;
	size_t		nb_cmds;

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

static int	create_pipe_tab(t_cmd *cmd, size_t i)
{
	int		prev_delim;
	size_t	j;
	size_t	index;

	j = 0;
	index = 0;
	prev_delim = cmd->prev_delim;
	while (j < i && g_signal == 0)
	{
		cmd->pipe[j] = add_pipe(cmd->cmd, &index, prev_delim, cmd);
		if (!cmd->pipe[j])
			return (1);
		prev_delim = PIPE;
		if (parse_cmd(cmd->pipe[j]))
			return (1);
		j++;
	}
	return (0);
}

int	parse_pipe(t_cmd *cmd)
{
	size_t	i;

	if (!cmd || !cmd->cmd)
		return (1);
	i = count_pipes(cmd->cmd);
	if (i == 1)
		return (0);
	cmd->pipe = (t_cmd **)ft_calloc((i + 1), sizeof(t_cmd *));
	if (!cmd->pipe)
		return (1);
	if (create_pipe_tab(cmd, i))
		return (1);
	return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 15:06:08 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 15:08:21 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*add_pipe(char *cmd, size_t *i)
{
	int			is_tok;
	t_tokens	toks;
	size_t		j;
	t_cmd		*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	init_tokens(&toks);
	j = 0;
	while (cmd[*i + j])
	{
		is_tok = save_token(cmd[*i + j], &toks);
		if (is_tok == 2)
			return (NULL);
		else if (!is_tok && cmd[*i + j] == '|' && !toks.par && !toks.dbl_qu
			&& !toks.spl_qu)
			break ;
		j++;
	}
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	*i += j + 1;
	return (new_cmd);
}

static long long	count_pipes(char *cmd)
{
	int			is_tok;
	t_tokens	toks;
	size_t		i;
	long long	nb_cmds;

	init_tokens(&toks);
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		is_tok = save_token(cmd[i], &toks);
		if (is_tok == 2)
			return (0);
		else if (!is_tok && cmd[i] == '|' && !toks.par && !toks.dbl_qu
			&& !toks.spl_qu)
			nb_cmds++;
		i++;
	}
	if (toks.par || toks.dbl_qu || toks.spl_qu || cmd[i - 1] == '|' 
		|| cmd[i - 1] == '&')
		return (0);
	nb_cmds++;
	return (nb_cmds);

}

int	parse_pipe(t_cmd *cmd)
{
	size_t		index;
	long long	i;
	long long	j;

	if (!cmd || !cmd->cmd)
		return (1);
	if (cmd->cmd[0] == '&' || cmd->cmd[0] == '|')
		return (write(2, "Parsing error\n", 14), 1);
	i = count_pipes(cmd->cmd);
	if (!i)
		return (write(2, "Parsing error\n", 14), 1);
	else if (i == 1)
		return (0);
	cmd->pipe = (t_cmd **)malloc(sizeof(t_cmd *) * (i + 1));
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
	cmd->pipe[0]->prev_delim = 0;
	cmd->pipe[i - 1]->next_delim = 0;
	cmd->pipe[i] = NULL;
	return (i);
}

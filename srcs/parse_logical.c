/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_logical.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:27:15 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/21 12:28:19 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*add_cmd(char *cmd, int *i)
{
	int			is_tok;
	t_tokens	toks;
	int			j;
	t_cmd		*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	init_tokens(&toks);
	skip_whitespaces(cmd, i);
	j = 0;
	while (cmd[*i + j])
	{
		is_tok = save_token(cmd[*i + j], &toks);
		if (is_tok == 2)
			return (NULL);
		else if (!is_tok && is_delim(&cmd[*i + j], 2) && !has_tokens(toks))
			break ;
		j++;
	}
	save_delim(new_cmd, cmd[*i + j]);
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	if (!new_cmd->cmd)
		return (free(new_cmd), write(2, "Parsing error\n", 14), NULL);
	*i += j + 2;
	return (new_cmd);
}

static long long	count_cmds(char *cmd)
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
		else if (!is_tok && is_delim(&cmd[i], 2) && !has_tokens(toks))
		{
			nb_cmds++;
			i++;
		}
			i++;
	}
	if (has_tokens(toks) || cmd[i - 1] == '|' || cmd[i - 1] == '&')
		return (0);
	nb_cmds++;
	return (nb_cmds);
}

int	parse_logical(t_cmd *cmd)
{
	int			index;
	long long	i;
	long long	j;

	if (!cmd || !cmd->cmd)
		return (1);
	if (cmd->cmd[0] == '&' || cmd->cmd[0] == '|')
		return (write(2, "Parsing error\n", 14), 1);
	i = count_cmds(cmd->cmd);
	if (!i)
		return (write(2, "Parsing error\n", 14), 1);
	else if (i == 1)
		return (0);
	cmd->sub_cmd = (t_cmd **)malloc(sizeof(t_cmd *) * (i + 1));
	if (!cmd->sub_cmd)
		return (1);
	j = 0;
	index = 0;
	while (j < i)
	{
		cmd->sub_cmd[j] = add_cmd(cmd->cmd, &index);
		if (!cmd->sub_cmd[j])
			return (1);
		if (j != 0)
			cmd->sub_cmd[j]->prev_delim = cmd->sub_cmd[j - 1]->next_delim;
		if (parse_cmd(cmd->sub_cmd[j]))
			return (1);
		j++;
	}
	cmd->sub_cmd[0]->prev_delim = 0;
	cmd->sub_cmd[i - 1]->next_delim = 0;
	cmd->sub_cmd[i] = NULL;
	return (i);
}

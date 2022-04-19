/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_logical.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:27:15 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 14:44:29 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*add_cmd(char *cmd, size_t *i, int prev_delim, t_cmd *father)
{
	t_tokens	toks;
	size_t		j;
	t_cmd		*new_cmd;

	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	init_tokens(&toks);
	j = 0;
	while (cmd[*i + j])
	{
		if (!save_token(cmd[*i + j], &toks) && is_delim(&cmd[*i + j], 2)
			&& !has_tokens(toks))
			break ;
		j++;
	}
	save_delim(new_cmd, &cmd[*i + j], prev_delim, father->next_delim);
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	if (!new_cmd->cmd)
		return (parsing_error(new_cmd->next_delim, NULL, NULL), free(new_cmd), \
		NULL);
	cmd[*i + j] = ' ';
	*i += j + 2;
	return (new_cmd);
}

static size_t	count_cmds(char *cmd)
{
	int			is_tok;
	t_tokens	toks;
	size_t		i;
	size_t		nb_cmds;

	init_tokens(&toks);
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		is_tok = save_token(cmd[i], &toks);
		if (is_tok == 2)
			return (parsing_error(PARENTH, NULL, NULL), 0);
		else if (!is_tok && is_delim(&cmd[i], 2) && !has_tokens(toks))
		{
			nb_cmds++;
			i++;
		}
			i++;
	}
	if (has_tokens(toks))
		return (parsing_error(-1, NULL, &toks), 0);
	nb_cmds++;
	return (nb_cmds);
}

static int	check_first_char(char *str)
{
	if (str[0] == '&' && str[1] == '&')
		return (parsing_error(AND, NULL, NULL), 1);
	else if (str[0] == '|' && str[1] == '|')
		return (parsing_error(OR, NULL, NULL), 1);
	else if (str[0] == '|')
		return (parsing_error(PIPE, NULL, NULL), 1);
	else if (str[0] == ')')
		return (parsing_error(PARENTH, NULL, NULL), 1);
	return (0);
}

static int	create_logical_tab(t_cmd *cmd, size_t i)
{
	int		prev_delim;
	size_t	j;
	size_t	index;

	prev_delim = cmd->prev_delim;
	j = 0;
	index = 0;
	while (j < i && g_signal == 0)
	{
		cmd->sub_cmd[j] = add_cmd(cmd->cmd, &index, prev_delim, cmd);
		if (!cmd->sub_cmd[j])
			return (1);
		prev_delim = cmd->sub_cmd[j]->next_delim;
		if (parse_cmd(cmd->sub_cmd[j]))
			return (1);
		j++;
	}
	return (0);
}

int	parse_logical(t_cmd *cmd)
{
	size_t	i;

	if (!cmd || !cmd->cmd)
		return (1);
	if (check_first_char(cmd->cmd))
		return (1);
	i = count_cmds(cmd->cmd);
	if (!i)
		return (1);
	else if (i == 1)
		return (0);
	cmd->sub_cmd = (t_cmd **)ft_calloc((i + 1), sizeof(t_cmd *));
	if (!cmd->sub_cmd)
		return (1);
	if (create_logical_tab(cmd, i))
		return (1);
	return (i);
}

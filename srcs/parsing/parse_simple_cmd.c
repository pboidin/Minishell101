/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 17:52:21 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_assignation(char	*str)
{
	size_t	j;
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	if (str[i] != '=')
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	j = 1;
	while (j < i)
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		j++;
	}
	return (1);
}

static int	is_valid_assignation(char *str)
{
	size_t	i;
	size_t	j;
	int		spl_qu;
	int		dbl_qu;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	j = i;
	spl_qu = 0;
	dbl_qu = 0;
	if (str[j] == '(')
	{
		j++;
		while (str[j])
		{
			if (str[j] == 39 && !dbl_qu)
				spl_qu ^= 1;
			else if (str[j] == '"' && !spl_qu)
				dbl_qu ^= 1;
			else if (!dbl_qu && !spl_qu && (str[j] == '<' || str[j] == '>'
				|| str[j] == '|' || str[j] == '&' || str[j] == '('))
				return (write(2, "Assignation error\n", 18), 0);
			else if(!dbl_qu && !spl_qu && str[j] == ')')
				break ;
			j++;
		}
		if (str[j] != ')' || dbl_qu || spl_qu)
			return (write(2, "Assignation error\n", 18), 0);
		j++;
	}
	while (str[j])
	{
		if (str[j] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[j] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[j] == '<' || str[j] == '>'
			|| str[j] == '|' || str[j] == '&' || str[j] == '(' || str[j] == ')'))
			return (write(2, "Assignation error\n", 18), 0);
		j++;
	}
	if (dbl_qu || spl_qu)
		return (write(2, "Assignation error\n", 18), 0);
	return (1);
}

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

int	is_valid_arg(char *str)
{
	size_t	i;
	int		spl_qu;
	int		dbl_qu;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[i] == '(' || str[i] == ')'
				|| str[i] == '|' || (str[i] == '&' && str[i + 1] == '&')
				|| str[i] == '<' || str[i] =='>'))
			return (0);
		i++;
	}
	return (1);
}

static void	clean_previous_args(t_cmd *cmd, int *i)
{
	int	j;

	j = 0;
	while (j < *i)
	{
		free(cmd->cmd_args[j]);
		j++;
	}
	move_upward(cmd, *i, *i);
	*i = 0;
}

static int	check_cmd(t_cmd *cmd)
{
	int		i;
	int		has_cmd;
	int		is_export;

	has_cmd = 0;
	i = 0;
	is_export = 0;
	while (cmd->cmd_args[i])
	{
		if ((!has_cmd || is_export) && is_assignation(cmd->cmd_args[i]))
		{
			if (!is_valid_assignation(cmd->cmd_args[i]))
				return (write(2, "Invalid assignation\n", 20), 1);
		}
		else if (cmd->cmd_args[i][0] == '>' || cmd->cmd_args[i][0] == '<')
		{
			if (parse_redirections(cmd, &i))
				return (1);
		}
		else if (!is_valid_arg(cmd->cmd_args[i]))
			return (parsing_error(-1, cmd->cmd_args[i], NULL), 1);
		else
		{
			if(!has_cmd)
			{
				if (!strcmp("export", cmd->cmd_args[i]))
					is_export = 1;
				clean_previous_args(cmd, &i);
			}
			has_cmd = 1;
		}
		i++;
	}
	return (0); 
}

int	parse_simple_cmd(t_cmd *cmd)
{
	int	nb_args;

	nb_args = parse_args(cmd, cmd->cmd);
	if (!nb_args)
		return (1);
	if(check_cmd(cmd))
		return (1);
	return (0);
}

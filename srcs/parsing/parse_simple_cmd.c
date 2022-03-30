/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 20:26:23 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/30 17:01:49 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_redirect(char *str, t_cmd *cmd, int redirect)
{
	t_redirect	*current;
	t_redirect	*new_redirect;

	new_redirect = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!new_redirect)
		return (perror("Malloc error"), 1);
	new_redirect->type = redirect;
	new_redirect->fd = -1;
	if (!cmd->redirections)
		cmd->redirections = new_redirect;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = new_redirect;
	}
	new_redirect->str = ft_trim(str);
	if (!new_redirect->str)
		return (perror("Malloc error"), 1);
	if (new_redirect->type == -2)
	{
		if (save_heredoc(new_redirect))
			return (1);
	}
	return (0);
}

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
	j = 0;
	while (j < i)
	{
		if (str[i] == '?' || str[i] == '&' || str[i] == '|' || str[i] == '('
			|| str[i] == ')' || str[i] == '"' || str[i] == 39 || str[i] == '$')
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

	while (str[i] && str[i] != '=')
		i++;
	i++;
	j = i;
	if (str[j] = '(')
	{
		spl_qu = 0;
		dbl_qu = 0;
		while (str[j])
		{
			if (str[j] == 39 && !dbl_qu)
				spl_qu ^= 1;
			else if (str[j] == '"' && !spl_qu)
				dbl_qu ^= 1;
			else if (!dbl_qu && !spl_qu && str[j] == '<' || str[j] == '>'
				|| str[j] == '|' || str[j] == '&' || str[j] == '(')
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
		else if (!dbl_qu && !spl_qu && str[j] == '<' || str[j] == '>'
			|| str[j] == '|' || str[j] == '&' || str[j] == '(' || str[j] == ')')
			return (write(2, "Assignation error\n", 18), 0);
		j++;
	}
	if (dbl_qu || spl_qu)
		return (write(2, "Assignation error\n", 18), 0);
	return (1);
}

static void	move_upward(t_cmd *cmd, size_t i, int mv)
{
	while (cmd->cmd_args[i])
	{
		cmd->cmd_args[i - mv] = cmd->cmd_args[i];
		j++:
	}
	while (mv)
	{
		cmd->cmd_args[i - mv] = NULL;
		mv--;
	}
}

static int	handle_redirections(t_cmd *cmd, int *i)
{
	char	str;
	int		redirect;
	int		j;
	int		spl_qu;
	int		dbl_qu;

	if (ft_strlen(cmd->cmd_args[*i] == 2))
	{
		if (cmd->cmd_args[*i][0] == '<')
			redirect == -2;
		else
			redirect == 2;
	}
	else
	{
		if (cmd->cmd_args[*i][0] == '<')
			redirect == -1;
		else
			redirect == 1;
	}
	if (!cmd->cmd_args[*i + 1])
		return (write(2, "No file given to redirect\n", 26), 1);
	str = cmd->cmd_args[*i + 1];
	if (str[0] == '>' || str[0] == '<')
		return (write(2, "Unexpected character in redirect\n", 33), 1);
	spl_qu = 0;
	dbl_qu = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[j] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!dbl_qu && !spl_qu && (str[j] == '(' || str[j] == ')' || str[j] == '|' || str[j] == '&'))
			return (write(2, "Unexpected character in redirect\n", 33), 1);
		j++;
	}
	if (add_redirect(str, cmd, redirect))a
		return (1);
	free(cmd->cmd_args[*i]);
	free(cmd->cmd_args[*i + 1]);
	cmd->cmd_args[*i] = NULL;
	cmd->cmd_args[*i + 1] = NULL;
	move_upward(cmd, (*i + 2), 2);
	*i -= 1;
	return (0);
}

static int	has_illegal_char(char *str)
{
	size_t	i;
	int		dbl_qu;
	int		spl_qu;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
			spl_qu ^= 1;
		else if (str[i] == '"' && !spl_qu)
			dbl_qu ^= 1;
		else if (!spl_qu && !dbl_qu && (str[i] == '(' || str[i] == ')' || str[i] == '|' || str[i] == '&'))
			return (perror("Unexpected character"), 1);
		i++;
	}
	if (spl_qu || dbl_qu)
		return (perror("Unclosed quotes"), 1);
	return (0);
}

char	*get_cmd_val(char *str)
{
	int		spl_qu;
	int		dbl_qu;
	size_t	i;
	char	*word;

	i = 0;
	spl_qu = 0;
	dbl_qu = 0;
	while (str[i])
	{
		if (str[i] == 39 && !dbl_qu)
		{
			str[i] = ' ';
			spl_qu ^= 1;
		}
		else if (str[i] == '"' && !spl_qu)
		{
			str[i] = ' ';
			dbl_qu ^= 1;
		}
		i++;
	}
	word = ft_del_spaces(redir->str);
	if (!word)
		return (NULL);
	printf("cmd %s\n", word);
	return (word);
}

static void	clean_previous_args(t_cmd *cmd, int *i)
{

}

static int	check_cmd(t_cmd *cmd)
{
	int		i;
	int		has_cmd;
	int		is_export;
	char	*cmd_val;

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
			if (handle_redirections(cmd, &i))
				return (1);
		}
		else if (has_illegal_char(cmd->cmd_args[i]))
			return (1);
		else
		{
			if(!has_cmd)
			{
				cmd_val = get_cmd_val(cmd->cmd_args[i]);
				if (!cmd_val)
					return (perror("Malloc error"), 1);
				if (!strncmp("export", cmd_val, 7))
					is_export = 1;
				free(cmd_val);
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
	int		nb_args;
	char	*trimmed;
	int		i;

	nb_args = parse_args(cmd);
	if (!nb_args)
		return (1);
	if(check_cmd(cmd))
		return (1);
	return (0);
}

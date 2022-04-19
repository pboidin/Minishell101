/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 17:35:17 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 14:51:39 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_redirect(char *str, t_cmd *cmd, int redirect)
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
	new_redirect->str = ft_strdup(str);
	if (!new_redirect->str)
		return (perror("Malloc error"), 1);
	if (new_redirect->type == -2 && save_heredoc(new_redirect))
		return (1);
	return (0);
}

int	get_redirect_type(char *str)
{
	int	redirect;

	redirect = 0;
	if (ft_strlen(str) == 2)
	{
		if (str[0] == '<')
			redirect = -2;
		else
			redirect = 2;
	}
	else
	{
		if (str[0] == '<')
			redirect = -1;
		else
			redirect = 1;
	}
	return (redirect);
}

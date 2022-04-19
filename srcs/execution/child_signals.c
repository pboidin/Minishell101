/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 11:59:43 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 15:30:40 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_signal(int signal)
{
	if (signal == SIGINT)
		g_signal = 130;
	else if (signal == SIGQUIT)
		g_signal = 131;
}

void	heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 250;
		close(0);
	}
}

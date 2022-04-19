/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:31:41 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 13:13:57 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = -1;

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		if (g_signal == -1 || g_signal == 130 || g_signal == 131)
		{

			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		if (g_signal == -2)
			close(0);
		g_signal = 130;
	}
	if (signal == SIGQUIT)
	{
		printf("Quit\n");
		g_signal = 131;
	}
}

int main(int argc, char **argv, char **envp)
{
	char				*cmd;
	t_info				info;

	if (argc != 1 || create_info(&info, envp, argv[0]))
		return (1);
	signal(SIGINT, handle_signal);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_signal = -1;
		cmd = readline("Minishell: ");
		signal(SIGQUIT, handle_signal);
		if (g_signal > 0)
			info.status = g_signal;
		if (cmd == NULL)
			return (free_info(&info), write(2, "exit\n", 5), info.status);
		if (cmd[0] != '\0')
		{
			g_signal = 0;
			add_history(cmd);
			info.cmd.cmd = ft_trim(cmd);
			free(cmd);
			if (info.cmd.cmd)
			{
				info.cmd.prev_delim = 0;
				info.cmd.next_delim = 0;
				if (!parse_cmd(&info.cmd))
					general_controller(&info, &info.cmd);
				else
					info.status = 2;
				free_cmd(&info.cmd);
				init_cmd(&info.cmd);
			}
		}
		else
			free(cmd);
		if (g_signal > 0)
			info.status = g_signal;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:31:41 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 15:04:01 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

void	handle_signal(int signal)
{
	if (signal == SIGKILL)
		g_signal = 1;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int main(int argc, char **argv, char **envp)
{
	char				*cmd;
	t_info				info;

	if (argc != 1 || create_info(&info, envp, argv[0]))
		return (1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	while (1)
	{
		cmd = readline("Minishell: ");
		if (cmd == NULL)
			return (free_info(&info), write(1, "exit\n", 5), info.status);
		if (cmd[0] != '\0')
		{
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
	}
	return (0);
}

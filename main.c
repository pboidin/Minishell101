/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:31:41 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/22 10:59:42 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

char *print_delim(int delim)
{
	 return (delim == 0 ? ("") : (delim == 1 ? ("|") : (delim == 2 ? ("&&") : ("||"))));
}

void	execute_command(t_cmd *cmd)
{
	t_redirect	*current;
	long long	i;

	i = 0;
	if (cmd->sub_cmd)
	{
		while (cmd->sub_cmd[i])
		{
			execute_command(cmd->sub_cmd[i]);
			printf("%s", print_delim(cmd->sub_cmd[i]->next_delim));
			i++;
		}
	}
	else if (cmd->pipe)
	{
		while (cmd->pipe[i])
		{
			execute_command(cmd->pipe[i]);
			if (cmd->pipe[i + 1])
				printf("|");
			i++;
		}
	}
	else if (cmd->fork)
	{
		printf("(");
		execute_command(cmd->fork);
		printf(")");
		current = cmd->in;
		while (current)
		{
			printf(" %s %s", (current->type == 1 ? "<" : "<<"), current->str);
			current = current->next;
		}
		current = cmd->out;
		while (current)
		{
			printf(" %s %s", (current->type == 1 ? ">" : ">>"), current->str);
			current = current->next;
		}
	}
	else
	{
		while (cmd->cmd_args[i])
		{
			printf("%s ", cmd->cmd_args[i]);
			i++;
		}
		current = cmd->in;
		while (current)
		{
			printf(" %s %s", (current->type == 1 ? "<" : "<<"), current->str);
			current = current->next;
		}
		current = cmd->out;
		while (current)
		{
			printf(" %s %s", (current->type == 1 ? ">" : ">>"), current->str);
			current = current->next;
		}
	}
}

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
			return (free_info(&info), info.status);
		if (cmd[0] != '\0')
		{
			add_history(cmd);
			info.cmd.cmd = ft_trim(cmd);
			free(cmd);
			if (!info.cmd.cmd)
				free(info.cmd.cmd);
			else
			{
				info.cmd.prev_delim = 0;
				info.cmd.next_delim = 0;
				if (!parse_cmd(&info.cmd))
				{
					general_controller(&info, &info.cmd);
					printf("\n");
				}
				else
					write(2, "Error\n", 6);
				free_cmd(&info.cmd);
				init_cmd(&info.cmd);
			}
		}
		else
			free(cmd);
	}
	return (0);
}

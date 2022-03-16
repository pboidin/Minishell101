/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:31:41 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 18:47:31 by bdetune          ###   ########.fr       */
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
	}
	else
		printf("%s", cmd->cmd_name);
}

int	parse_simple_cmd(t_cmd *cmd)
{
	cmd->cmd_name = cmd->cmd;
	return (0);
}

int	init_cmd(t_cmd *cmd)
{
	char	*trimmed_cmd;

	trimmed_cmd = ft_trim(cmd->cmd);
	if (!trimmed_cmd)
		return (write(2, "Parsing error\n", 14), 1);
	free(cmd->cmd);
	cmd->cmd = trimmed_cmd;
	cmd->fork = NULL;
	cmd->pipe = NULL;
	cmd->sub_cmd = NULL;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->cmd_name = NULL;
	cmd->cmd_args = NULL;
	return (0);
}

int	parse_cmd(t_cmd *cmd)
{
	int		ret;

	if (init_cmd(cmd))
		return (1);
	ret = parse_logical(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = parse_pipe(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = fork_cmd(cmd);
	if (ret == 2)
		return (1);
	else if (ret)
		return (0);
	ret = parse_simple_cmd(cmd);
	if (ret)
		return (1);
	return (0);
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
			return (free_info(&info), info.last_ret);
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
					execute_command(&info.cmd);
					printf("\n");
				}
				else
					write(2, "Error\n", 6);
			}
		}
		else
			free(cmd);
	}
	return (0);
}

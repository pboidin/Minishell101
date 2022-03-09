/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:37:26 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/09 18:52:31 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int	main(int ac, char **av, char **envp)
{
	pid_t	pid;
	pid_t	pid2;
	int	fd[2];
	char	**args;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
//		close(fd[1]);
		args = malloc(sizeof(char**) * 2);
		args[0] = av[0];
		args[1] = NULL;
		execve("/bin/cat", args, envp);
		free(args);
		printf("Failed to find command)\n");
		return (1);
	}
	else
	{
		pid2 = fork();
		if (pid2 == 0)
		{
			dup2(fd[0], 0);
//			close(fd[0]);
			close(fd[1]);
			args = malloc(sizeof(char**) * 3);
			args[0] = av[0];
			args[1] = "()";
			args[2] = NULL;
			execve("/bin/ls", args, envp);
			free(args);
			printf("Failed to find command)\n");
			return (1);
		}
		else
		{
			int	status;

			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
			return (status);
		}
	}
	return (0);
}

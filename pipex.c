/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:37:26 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/11 12:06:46 by bdetune          ###   ########.fr       */
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
	char	*test;

	test = malloc(2);
	test[0] = 'a';
	test[1] = '\0';
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		free(test);
		test = malloc(3);
		test[0]='b';
		test[1]='a';
		test[2] = '\0';
		printf("Test2: %s\n", test);
		dup2(fd[1], 1);
		close(fd[0]);
//		close(fd[1]);
		args = malloc(sizeof(char**) * 2);
		args[0] = av[0];
		args[1] = NULL;
		execve("/bin/ls", args, envp);
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
			args = malloc(sizeof(char**) * 2);
			args[0] = av[0];
			args[1] = NULL;
			execve("/bin/wc", args, envp);
			free(args);
			printf("Failed to find command)\n");
			return (1);
		}
		else
		{
			int	status;
			
			printf("Test1: %s\n", test);
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
			free(test);
			return (status);
		}
	}
	return (0);
}

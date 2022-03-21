/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:22:58 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/21 11:23:41 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.c"

char	*ft_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*path_dir;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path_dir = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_dir, cmd);
		free(path_dir);
		if (access(path, F_OK) == 0)
			return (path);
		i++;
	}
	return (0);
}

void	ft_error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

void	ft_execute(char *argv, char **envp)
{
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (execve(ft_path(cmd[0], envp), cmd, envp) == -1)
		ft_error();
}

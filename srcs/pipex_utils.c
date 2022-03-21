/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:22:58 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/21 14:49:32 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_path(char *cmd, t_info *info)
{
	t_env	*current;
	char	**paths;
	char	*path;
	char	*path_dir;
	int		i;

	if (!info->env)
		return (NULL);
	current = info->env;
	while (current)
	{
		if (ft_strncmp(current->name, "PATH", 5))
			break ;
		current = current->next;
	}
	if (!current)
		return (NULL);
	paths = ft_split(current-value, ':');
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

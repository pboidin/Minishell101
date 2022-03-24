/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:22:58 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/22 14:24:03 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*ft_path(t_info *info, char *cmd)
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
		if (!ft_strncmp(current->name, "PATH", 5))
			break ;
		current = current->next;
	}
	if (!current)
		return (NULL);
	paths = ft_split(current->value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_dir = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_dir, cmd);
		free(path_dir);
		if (access(path, F_OK) == 0)
			return (free_split(paths), path);
		free(path);
		i++;
	}
	return (free_split(paths), NULL);
}

void	ft_error(t_info *info, char *cmd, char **envp)
{
	size_t	i;

	free_info(info);
	if (cmd)
		free(cmd);
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
	perror("Error");
	exit(EXIT_FAILURE);
}

void	ft_execute(t_info *info, char **cmd_args)
{
	char	**envp;
	char	*cmd;

	cmd = NULL;
	envp = NULL;
	cmd = ft_path(info, cmd_args[0]);
	if (!cmd)
		ft_error(info, NULL, NULL);
	envp = join_env(info);
	if (!envp)
		ft_error(info, cmd, NULL);
	if (execve(cmd, cmd_args, envp) == -1)
		ft_error(info, cmd, envp);
}
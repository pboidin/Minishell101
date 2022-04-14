/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:22:58 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/14 19:06:01 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path(t_info *info)
{
	char	**paths;
	t_env	*current;

	if (!info->env)
		return (NULL);
	current = info->env;
	while (current)
	{
		if (!ft_strcmp(current->name, "PATH"))
			break ;
		current = current->next;
	}
	if (!current || !current->value || current->value[0] == '\0')
		return (NULL);
	paths = ft_split(current->value, ':');
	if (!paths)
		return (execution_error(info, NULL, EXIT_FAILURE, 0), NULL);
	return (paths);
}

char	*ft_path(t_info *info, t_cmd *cmd)
{
	char	**paths;
	char	*path;
	char	*path_dir;
	int		i;

	paths = get_path(info);
	if (!paths)
		return (execution_error(info, cmd, 127, 0), NULL);;
	i = -1;
	while (paths[++i])
	{
		path_dir = ft_strjoin(paths[i], "/");
		if (!path_dir)
			return (free_char_tab(paths), \
				execution_error(info, NULL, EXIT_FAILURE, 0), NULL);
		path = ft_strjoin(path_dir, cmd->cmd_args[0]);
		free(path_dir);
		if (!path)
			return (free_char_tab(paths), \
				execution_error(info, NULL, EXIT_FAILURE, 0), NULL);
		if (!access(path, F_OK))
			return (free_char_tab(paths), path);
		free(path);
	}
	return (free_char_tab(paths), NULL);
}

static int	is_absolute(t_info *info, t_cmd *cmd)
{
	size_t	i;

	i = 0;
	if (!cmd->cmd_args[0][0])
		return (execution_error(info, cmd, 127, 0), 0);
	while (cmd->cmd_args[0][i])
	{
		if (cmd->cmd_args[0][i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	ft_execute(t_info *info, t_cmd *cmd)
{
	if (is_absolute(info, cmd))
	{
		cmd->cmd_name = ft_strdup(cmd->cmd_args[0]);
		if (!cmd->cmd_name)
			execution_error(info, cmd, EXIT_FAILURE, 1);
		if (access(cmd->cmd_name, F_OK))
			execution_error(info, cmd, 127, 1);
	}
	else
	{
		cmd->cmd_name = ft_path(info, cmd);
		if (!cmd->cmd_name)
			execution_error(info, cmd, 127, 0);
	}
	if (access(cmd->cmd_name, X_OK))
		execution_error(info, cmd, 126, 0);
	cmd->joined_env = join_env(info);
	if (!cmd->joined_env)
		execution_error(info, cmd, EXIT_FAILURE, 0);
	if (execve(cmd->cmd_name, cmd->cmd_args, cmd->joined_env) == -1)
		execution_error(info, cmd, EXIT_FAILURE, 0);
}

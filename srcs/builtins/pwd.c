/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:55 by piboidin          #+#    #+#             */
/*   Updated: 2022/04/10 19:38:26 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_genv(const char *path, t_env *info)
{
	t_env	*tmp;

	tmp = info;
	while (tmp)
	{
		if (!ft_strcmp((char *)path, tmp->name))
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

static void	ft_print_pwd(const char *pwd, int fd)
{
	write(fd, pwd, ft_strlen(pwd));
	write(fd, "\n", 1);
}

int	ft_working_dir(char **n, t_info *info, t_cmd *cmd)
{
	char	*pwd;
	int		fd;

	fd = 1;
	if (cmd->out)
		fd = cmd->out->fd;
	pwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!pwd)
		return (1);
	(void)n;
	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		free(pwd);
		pwd = NULL;
	}
	if (pwd == NULL)
		pwd = ft_genv("PWD", info->env);
	if (pwd == NULL)
		return (1);
	ft_print_pwd(pwd, fd);
	free(pwd);
	return (0);
}

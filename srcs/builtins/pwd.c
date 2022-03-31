/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:55 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:57 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_genv(const char *path, t_info *info)
{
	int		path_size;
	char	*env;
	char	*ret;
	t_env	*tmp;

	tmp = info->env;
	env = ft_strjoin(path, "=");
	path_size = ft_strlen(env);
	while (tmp)
	{
		if (ft_strncmp(env, (char *)tmp->value, path_size) == 0)
		{
			ret = ft_substr((char *)tmp->value, path_size,
					ft_strlen((char *)tmp->value) - path_size);
			free(env);
			return (ret);
		}
		tmp = tmp->next;
	}
	free(env);
	return (NULL);
}

static void	ft_print_pwd(const char *pwd)
{
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
}

int	ft_working_dir(char **n, t_info *info)
{
	char	*pwd;

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
		pwd = ft_genv("PWD", info);
	if (pwd == NULL)
		return (1);
	ft_print_pwd(pwd);
	free(pwd);
	return (0);
}

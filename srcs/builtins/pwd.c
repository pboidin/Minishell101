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
	char	*env;
	char	*ret;
	t_env	*tmp;

	tmp = info;
	env = ft_strjoin(path, "=");
	while (tmp)
	{
		if (!ft_strcmp((char *)path, tmp->name))
		{
			ret = ft_strdup(tmp->value);
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
		pwd = ft_genv("PWD", info->env);
	if (pwd == NULL)
		return (1);
	ft_print_pwd(pwd);
	free(pwd);
	return (0);
}

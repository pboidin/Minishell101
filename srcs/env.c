/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 14:23:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/22 11:34:34 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_env_var(t_info *info)
{
	t_env	*current;
	size_t	i;

	i = 0;
	current = info->env;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

static char	*join_env_var(t_env *env)
{
	char	*line;
	size_t	size;
	size_t	i;

	size = ft_strlen(env->name) + ft_strlen(env->value);
	line = (char *)malloc(sizeof(char) * (size + 2));
	size = 0;
	while (env->name[size])
	{
		line[size] = env->name[size];
		size++;
	}
	line[size] = '=';
	size++;
	i = 0;
	while (env->value[i])
		line[size++] = env->value[i++];
	line[size] = '\0';
	return (line);
}

char	**join_env(t_info *info)
{
	size_t	i;
	size_t	size;
	t_env	*current;
	char	**env_tab;

	size = count_env_var(info);
	if (!size)
	{
		env_tab = (char **)malloc(sizeof(char *));
		if (!env_tab)
			return (write (2, "Malloc error\n", 13), NULL);
		env_tab[0] = NULL;
		return (env_tab);
	}
	env_tab = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env_tab)
		return (write (2, "Malloc error\n", 13), NULL);
	i = 0;
	current = info->env;
	while (i < size)
	{
		env_tab[i] = join_env_var(current);
		if (!env_tab[i])
			return (NULL);
		current = current->next;
		i++;
	}
	env_tab[i] = NULL;
	return (env_tab);
}

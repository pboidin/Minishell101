/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 14:23:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/13 11:27:08 by bdetune          ###   ########.fr       */
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
	line = (char *)ft_calloc((size + 2), sizeof(char));
	if (!line)
		return (perror("Malloc error"), NULL);
	size = 0;
	while (env->name && env->name[size])
	{
		line[size] = env->name[size];
		size++;
	}
	line[size] = '=';
	size++;
	i = 0;
	while (env->value && env->value[i])
		line[size++] = env->value[i++];
	return (line);
}

char	**join_env(t_info *info)
{
	size_t	i;
	t_env	*current;
	char	**env_tab;

	env_tab = create_char_tab(count_env_var(info));
	if (!env_tab)
		return (NULL);
	i = 0;
	current = info->env;
	while (current)
	{
		env_tab[i] = join_env_var(current);
		if (!env_tab[i++])
			return (free_char_tab(env_tab), NULL);
		current = current->next;
	}
	return (env_tab);
}

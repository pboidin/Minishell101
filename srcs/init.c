/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 14:17:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/18 15:18:14 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_str(char *line, char sep, size_t *index)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = *index;
	j = 0;
	while (line[i + j] && line[i + j] != sep)
		j++;
	str = (char *)malloc(sizeof(char) * (j + 1));
	if (!str)
		return (NULL);
	j = 0;
	while (line[i + j] && line[i + j] != sep)
	{
		str[j] = line[i + j];
		j++;
	}
	*index += j + 1;
	return (str);
}

static int	add_to_env(t_info *info, char *line)
{
	size_t	index;
	t_env	*new_var;

	index = 0;
	new_var = (t_env *)malloc(sizeof(t_env));
	if (!new_var)
		return (1);
	new_var->name = get_str(line, '=', &index);
	if (!new_var->name)
		return (free(new_var), 1);
	new_var->value = get_str(line, '\0', &index);
	if (!new_var->value)
		return (free(new_var->name), free(new_var), 1);
	new_var->next = info->env;
	info->env = new_var;
	return (0);
}

int	create_info(t_info *info, char **envp, char *name)
{
	size_t	i;

	info->prog_name = name;
	info->local_var = NULL;
	info->running_processes = NULL;
	info->status = 0;
	info->env = NULL;
	if (envp)
	{
		i = 0;
		while (envp[i])
			i++;
		while (i--)
		{
			if (add_to_env(info, envp[i]))
				return (free_env(info), 1);
		}
	}
	return (0);
}

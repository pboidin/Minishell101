/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:17:36 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 15:35:13 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	compare_no_wild(char *path, t_block *mask)
{
	if (ft_strcmp(path, mask[0].str))
		return (0);
	return (1);
}

int	ft_compare(t_block *mask, char *path)
{
	int	i;

	if (!path || (path[0] == '.' && mask[0].str[0] != '.'))
		return (0);
	if (!mask[0].dbl_qu)
		return (compare_no_wild(path, mask));
	i = -1;
	if (!mask[0].var && ft_strncmp(path, mask[0].str, ft_strlen(mask[0].str)))
		return (0);
	while (mask[++i + 1].str)
	{
		if (!mask[i].var)
		{
			path = ft_strstr(path, mask[i].str);
			if (!path)
				return (0);
			path = &path[ft_strlen(mask[i].str)];
		}
	}
	if (mask[i].var || (ft_strlen(path) >= ft_strlen(mask[i].str)
			&& !ft_strcmp(mask[i].str, \
			&path[ft_strlen(path) - ft_strlen(mask[i].str)])))
		return (1);
	return (0);
}

DIR	*open_dir(const char *path, size_t *path_len)
{
	DIR		*dr;

	if (!path || g_signal > 0)
		return (NULL);
	*path_len = strlen(path);
	if (!path || !*path_len || (*path_len > _POSIX_PATH_MAX))
		return (NULL);
	dr = opendir(path);
	if (dr == NULL)
		return (NULL);
	return (dr);
}

int	ft_recur_sa_mere(struct dirent *de, t_wild **tmp,
		char *full_name, t_block **mask)
{
	if (end_wilderness(mask))
	{
		if (final_match(de->d_name, full_name, mask))
		{
			*tmp = add_to_list(*tmp, full_name, mask);
			if (!*tmp)
				return (1);
		}
		else
			free(full_name);
	}
	else if (ft_compare(mask[0], de->d_name))
	{
		if (!*tmp)
			*tmp = print_dirs(full_name, &mask[1]);
		else
			(ft_lstlast_wild(*tmp))->next = print_dirs(full_name, &mask[1]);
		free(full_name);
	}
	else
		free(full_name);
	return (0);
}

t_wild	*print_dirs(char *path, t_block **mask)
{
	t_wild			*tmp;
	struct dirent	*de;
	DIR				*dr;
	size_t			path_len;
	char			*full_name;

	dr = open_dir(path, &path_len);
	if (!dr)
		return (NULL);
	tmp = NULL;
	de = readdir(dr);
	while (de != NULL)
	{
		if (ft_compare(mask[0], de->d_name)
			&& (path_len + ft_strlen(de->d_name) + 1) <= _POSIX_PATH_MAX)
		{
			full_name = get_full_path(path, de->d_name, path_len);
			if (!full_name || ft_recur_sa_mere(de, &tmp, full_name, mask))
				return (closedir(dr), free(full_name), NULL);
		}
		de = readdir(dr);
	}
	closedir(dr);
	return (tmp);
}

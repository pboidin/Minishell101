/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:12:31 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/13 18:50:46 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_full_path(char *path, char *new_file, size_t path_len)
{
	char	*full_name;

	full_name = (char *)ft_calloc((_POSIX_PATH_MAX + 1), sizeof(char));
	if (!full_name)
	{
		g_signal = 1;
		perror("malloc error");
		return (NULL);
	}
	ft_strcpy(full_name, path);
	if (full_name[path_len - 1] != '/')
		ft_strcat(full_name, "/");
	ft_strcat(full_name, new_file);
	return (full_name);
}

t_block	*add_str_t_block(t_block *spl, char *str, int i, char wild)
{
	int		current;
	int		size;
	char	tmp;
	t_block	*new_block;

	size = 0;
	while (spl && spl[0].str && spl[size].str)
		size++;
	new_block = ft_calloc((size + 2), sizeof(t_block));
	if (!new_block)
		return (NULL);
	current = -1;
	while (++current < size)
		new_block[current] = spl[current];
	tmp = str[i];
	str[i] = '\0';
	new_block[current].str = ft_strdup(str);
	if (!new_block[current].str)
		return (free_t_block(new_block), free(spl), NULL);
	str[i] = tmp;
	if (wild)
		new_block[current].var = 1;
	return (free(spl), new_block);
}

t_block	*split_on_wild(t_block **new_block, t_block *block, int s[2])
{
	t_block	*spl;
	int		i;
	int		j;
	int		wild;

	spl = NULL;
	i = 0;
	j = 0;
	wild = 0;
	if (block[s[0]].str[s[1]] == '\0')
	{
		s[0] += 1;
		s[1] = 0;
	}
	while (new_block[0][0].str[i])
	{
		if (new_block[0][0].str[i] == '*' && block[s[0]].dbl_qu == 0
			&& block[s[0]].spl_qu == 0)
		{
			spl = add_str_t_block(spl, &new_block[0][0].str[j], (i - j), wild);
			if (!spl)
				return (free_t_block_tab(new_block), NULL);
			wild = 1;
			j = i;
		}
		else if (wild)
		{
			spl = add_str_t_block(spl, &new_block[0][0].str[j], (i - j), wild);
			if (!spl)
				return (free_t_block_tab(new_block), NULL);
			wild = 0;
			j = i;
		}
		i++;
		s[1] += 1;
		if (block[s[0]].str[s[1]] == '\0')
		{
			s[0] += 1;
			s[1] = 0;
		}
	}
	spl = add_str_t_block(spl, &new_block[0][0].str[j], (i - j), wild);
	if (!spl)
		return (free_t_block_tab(new_block), NULL);
	if (spl[0].str[0] == '\0')
		move_upward_t_block_str(spl, 0);
	spl[0].dbl_qu = -1;
	return (free(new_block[0][0].str), free(new_block[0]), spl);
}

t_wild	*get_lcl_files(t_block **mask, DIR *cur_dir)
{
	t_wild			*list;
	struct dirent	*de;

	list = NULL;
	de = readdir(cur_dir);
	while (de)
	{
		if (final_match(de->d_name, de->d_name, mask))
		{
			list = add_to_list(list, ft_strdup(de->d_name), mask);
			if (!list)
				return (closedir(cur_dir), NULL);
		}
		de = readdir(cur_dir);
	}
	return (closedir(cur_dir), list);
}

t_wild	*wild_start(t_block **mask)
{
	t_wild			*list;
	size_t			path_len;
	DIR				*cur_dir;
	struct dirent	*de;

	cur_dir = open_dir(".", &path_len);
	if (!cur_dir)
		return (NULL);
	list = NULL;
	if (end_wilderness(mask))
		return (get_lcl_files(mask, cur_dir));
	de = readdir(cur_dir);
	while (de)
	{
		if (ft_compare(mask[0], de->d_name))
		{
			if (!list)
				list = print_dirs(de->d_name, &mask[1]);
			else
				(ft_lstlast_wild(list))->next = \
					print_dirs(de->d_name, &mask[1]);
		}
		de = readdir(cur_dir);
	}
	return (closedir(cur_dir), list);
}

t_block	**t_wild_to_t_block_tab(t_wild *list)
{
	size_t	i;
	t_wild	*current;
	t_wild	*next;
	t_block	**ret;

	ret = ft_calloc((ft_lstlen(list) + 1), sizeof(t_block *));
	if (!ret)
		return (NULL);
	current = list;
	i = 0;
	while (current)
	{
		ret[i] = ft_calloc(2, sizeof(t_block));
		if (!ret[i])
		{
			g_signal = 1;
			return (free_t_block_tab(ret), NULL);
		}
		ret[i][0].str = current->path;
		next = current->next;
		free(current);
		current = next;
		i++;
	}
	return (ret);
}

t_block	**wild_one(t_block *block)
{
	t_wild	*list;
	t_block	**mask;

	if (!block || !block[0].str || !ft_has_wildcards(block))
		return (NULL);
	delete_empty(block);
	mask = build_mask(block);
	if (!mask)
		return (NULL);
	if (mask[0][0].dbl_qu)
		list = wild_start(mask);
	else
		list = print_dirs(mask[0][0].str, &mask[1]);
	free_t_block_tab(mask);
	if (!list)
		return (NULL);
	mask = t_wild_to_t_block_tab(list);
	return (mask);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:12:31 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/11 20:31:02 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_compare(char *s1, t_block *s2, int i, int j)
{
	while (!s2[i].str[j] && s2[i + 1].str)
	{
		i++;
		j = 0;
	}
	if (!*s1 && !s2[i].str[j])
		return (1);
	else if (*s1 == s2[i].str[j] && *s1 != '*')
		return (ft_compare(s1 + 1, s2, i, (j + 1)));
	else if (*s1 == '*' && s2[i].str[j] == '*')
		return (ft_compare(s1 + 1, s2, i, j));
	else if (s2[i].str[j] && !*s1)
		return (ft_compare(s1, s2, i, (j + 1)));
	else if (s2[i].str[j] == '*' && s2[i].str[j] && *s1)
		return (ft_compare(s1, s2, i, (j + 1)) || ft_compare(s1 + 1, s2, i, j));
	else
		return (0);
}

DIR	*open_dir(const char *path, size_t *path_len)
{
	DIR		*dr;

	if (!path)
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
		char *full_name, int depth[2])
{
	struct stat	fstat;
	t_wild		*last;
	t_wild		*new;

	if (!((ft_strcmp(de->d_name, ".") == 0) || (ft_strcmp(de->d_name, "..") == 0))
		&& !(stat(full_name, &fstat) < 0) && (S_ISDIR(fstat.st_mode)
			|| S_ISREG(fstat.st_mode)))
	{
		if (depth[0] == depth[1])
		{
			new = ft_lstnew_wild(full_name);
			if (!new)
				return (free(full_name), 1);
			if (!(*tmp))
				*tmp = new;
			else
			{
				last = ft_lstlast_wild(*tmp);
				last->next = new;
			}
		}
		else
		{
			depth[0] += 1;
			if (depth[0] <= depth[1])
			{
				if (!(*tmp))
					*tmp = print_dirs(full_name, depth);
				else
				{
					last = ft_lstlast_wild(*tmp);
					last->next = print_dirs(full_name, depth);
				}
			}
			depth[0] -= 1;
		}
	}
	else
		free(full_name);
	return (0);
}

t_wild	*print_dirs(char *path, int depth[2])
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
		full_name = (char *)calloc((_POSIX_PATH_MAX + 1), sizeof(char));
		if ((path_len + strlen(de->d_name) + 1) > _POSIX_PATH_MAX)
			continue ;
		ft_strcpy(full_name, path);
		if (full_name[path_len - 1] != '/')
			ft_strcat(full_name, "/");
		ft_strcat(full_name, de->d_name);
		if (ft_recur_sa_mere(de, &tmp, full_name, depth))
			return (closedir(dr), NULL);
		de = readdir(dr);
	}
	closedir(dr);
	return (tmp);
}
/*
t_block	*mask(t_block *block)
{
	t_block			*ret;
	unsigned int	i;
	unsigned int	j;


	ret = NULL;
	i = 0;
	while (block[i].str[0] == '\0')
		i++;

}
*/
char	**wild_one(t_block *block)
{
	char	*argt;
	int		depth[2];
//	char	**tab;
	t_wild	*list;
//	t_block	*mask;
//	int		i;
//	int		j;

//	i = 0;
//	tab = NULL;
//	list = NULL;
//	argt = NULL;
	if (!block || !block[0].str || !ft_has_wildcards(block))
		return (NULL);
	else
	{
		printf("Has wildcard\n");
	}
	argt = ft_delete_wild(block, &depth[1]);
	printf("Folder to open: %s, found depth: %d\n", argt, depth[1]);
	depth[0] = 0;
	list = print_dirs(argt, depth);
	if (!list)
		return (free(argt), NULL);
//	mask = build_mask(block);
	while (list)
	{
		printf("candidate: %s\n", list->path);
		list = list->next;
	}
	return (NULL);
	/*

	j = ft_lstlen(list, block);
	tab = (char **)malloc(sizeof(char *) * (j + 1));
	if (!tab)
		return (1);
	ft_wild_two(block, argt, &tab, &list);
	i = -1;
	while (tab[++i])
		printf("tab[%d] : %s\n", i, tab[i]);
	return (0);*/
}

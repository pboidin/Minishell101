/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:12:31 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/02 15:57:54 by piboidin         ###   ########.fr       */
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
		char *full_name, int recursive)
{
	struct stat	fstat;
	t_wild		*last;
	t_wild		*new;

	if (!((ft_strcmp(de->d_name, ".") == 0) || (ft_strcmp(de->d_name, "..") == 0))
		&& !(stat(full_name, &fstat) < 0) && (S_ISDIR(fstat.st_mode)
			|| S_ISREG(fstat.st_mode)))
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
		if (recursive)
		{
			last = ft_lstlast_wild(*tmp);
			last->next = print_dirs(full_name, 1);
		}
	}
	else
		free(full_name);
	return (0);
}

t_wild	*print_dirs(char *path, int recursive)
{
	t_wild		*tmp;
	struct dirent	*de;
	DIR			*dr;
	size_t		path_len;
	char		*full_name;

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
		if (ft_recur_sa_mere(de, &tmp, full_name, recursive))
			return (closedir(dr), NULL);
	}
	closedir(dr);
	return (tmp);
}

int	wild_one(int argc, char **argv, t_block *block)
{
	char	*argt;
	char	**tab;
	t_wild	*list;
	int		i;
	int		j;

	i = 0;
	tab = NULL;
	list = NULL;
	argt = NULL;
	if (argc != 2)
		return (0);
	if (!ft_has_wildcards(block))
		return (1);
	while (block[i].str)
	{
		if (ft_strchr_wild(block[i].str, '/'))
		{
			argt = ft_delete_wild(block);
			break ;
		}
		i++;
	}
	if (!argt)
	{
		argt = (char *)malloc(sizeof(char) * PATH_MAX);
		if (!argt || getcwd(argt, PATH_MAX) == NULL)
			return (free(argt), 1);
	}
	list = print_dirs (argt, 1);
	j = ft_lstlen(list, block);
	tab = (char **)malloc(sizeof(char *) * (j + 1));
	if (!tab)
		return (1);
	ft_wild_two(block, argt, &tab, &list);
	i = -1;
	while (tab[++i])
		printf("tab[%d] : %s\n", i, tab[i]);
	return (0);
}

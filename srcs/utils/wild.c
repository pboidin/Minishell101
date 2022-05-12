/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:12:31 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/12 14:59:43 by bdetune          ###   ########.fr       */
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

	printf("Wildcard\n");
	spl = NULL;
	i = 0;
	j = 0;
	wild = 0;
	while (new_block[0][0].str[i])
	{
		if (new_block[0][0].str[i] == '*' && block[s[0]].dbl_qu == 0 && block[s[0]].spl_qu == 0)
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

t_block	**add_to_mask(int s[2], int e[2], char wild, t_block *block)
{
	t_block	**new_block;
	char	tmp;
	int		c[2];

	(void)wild;
	new_block = ft_calloc(2, sizeof(t_block *));
	if (!new_block)
		return (NULL);
	if (e[0] == 0 && e[1] == 0)
	{
		new_block[0] = ft_calloc(2, sizeof(t_block));
		if (!new_block[0])
			return (free(new_block), NULL);
		new_block[0][0].str = ft_calloc(2, sizeof(char));
		if (!new_block[0][0].str)
			return (free_t_block_tab(new_block), NULL);
		new_block[0][0].str[0] = '/';
		return (new_block);
	}
	new_block[0] = ft_calloc(2, sizeof(t_block));
	if (!new_block[0])
		return (free(new_block), NULL);
	if (s[0] == e[0])
	{
		tmp = block[e[0]].str[e[1]];
		block[e[0]].str[e[1]] = '\0';
		new_block[0][0].str = ft_strdup(&block[s[0]].str[s[1]]);
		block[e[0]].str[e[1]] = tmp;
		if (!new_block[0][0].str)
			return (free_t_block_tab(new_block), NULL);
		if (wild)
			new_block[0] = split_on_wild(new_block, block, s);
		return (new_block);
	}
	new_block[0][0].str = ft_strdup(&block[s[0]].str[s[1]]);
	if (!new_block[0][0].str)
		return (free_t_block_tab(new_block), NULL);
	c[0] = s[0] + 1;
	while (c[0] != e[0])
	{
		new_block[0][0].str = ft_strcat_mal(new_block[0][0].str, block[c[0]].str);
		if (!new_block[0][0].str)
			return (free_t_block_tab(new_block), NULL);
		c[0] += 1;
	}
	tmp = block[e[0]].str[e[1]];
	block[e[0]].str[e[1]] = '\0';
	new_block[0][0].str = ft_strcat_mal(new_block[0][0].str, block[c[0]].str);
	block[e[0]].str[e[1]] = tmp;
	if (!new_block[0][0].str)
		return (free_t_block_tab(new_block), NULL);
	if (wild)
		new_block[0] = split_on_wild(new_block, block, s);
	return (new_block);
}

t_block	**build_mask(t_block *block)
{
	t_block	**ret;
	t_block	**tmp;
	int		s[2];
	int		e[2];
	char	wild;


	ret = NULL;
	s[0] = 0;
	s[1] = 0;
	e[0] = 0;
	e[1] = 0;
	wild = 0;
	while (block[e[0]].str)
	{
		e[1] = 0;
		while (block[e[0]].str[e[1]])
		{
			if (block[e[0]].str[e[1]] == '*' && block[e[0]].dbl_qu == 0 && block[e[0]].spl_qu == 0)
				wild = 1;
			if (block[e[0]].str[e[1]] == '/')
			{
				tmp = add_to_mask(s, e, wild, block);
				if (!tmp)
					return (free_t_block_tab(ret), NULL);
				ret = add_block_to_tab(ret, tmp);
				if (!ret)
					return (NULL);
				s[0] = e[0];
				s[1] = e[1] + 1;
				wild = 0;
			}
			e[1] += 1;
		}
		e[0] += 1;
	}
	e[0] -= 1;
	e[1] = ft_strlen(block[e[0]].str);
	tmp = add_to_mask(s, e, wild, block);
	if (!tmp)
		return (free_t_block_tab(ret), NULL);
	ret = add_block_to_tab(ret, tmp);
	if (!ret)
		return (NULL);
	return (ret);
}

void	delete_empty(t_block *block)
{
	int	i;

	i = 0;
	while (block[i].str)
	{
		if (block[i].str[0] == '\0')
		{
			move_upward_t_block_str(block, i);
			i--;
		}
		i++;
	}
}

char	**wild_one(t_block *block)
{
//	char	*argt;
//	int		depth[2];
//	char	**tab;
//	t_wild	*list;
	t_block	**mask;
	int		i;
	int		j;

//	i = 0;
//	tab = NULL;
//	list = NULL;
//	argt = NULL;
	if (!block || !block[0].str || !ft_has_wildcards(block))
		return (NULL);
	else
		printf("Has wildcard\n");
	i = 0;
	printf("Before mending\n");
	while (block[i].str)
	{
		printf("%s\n", block[i].str);
		i++;
	}
	delete_empty(block);
	i = 0;
	printf("After mending\n");
	while (block[i].str)
	{
		printf("%s\n", block[i].str);
		i++;
	}
	mask = build_mask(block);
	if (!mask)
		return (NULL);
	i = 0;
	printf("After splitting on folders\n");
	while (mask[i])
	{
		j = 0;
		while (mask[i][j].str)
		{
			if (mask[i][j].var)
				printf(" wildcard ");
			else
				printf(" %s ", mask[i][j].str);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("-----------------\n");
/*	argt = ft_delete_wild(block, &depth[1]);
	printf("Folder to open: %s, found depth: %d\n", argt, depth[1]);
	depth[0] = 0;
	list = print_dirs(argt, depth);
	if (!list)
		return (free(argt), NULL);
	while (list)
	{
		printf("candidate: %s\n", list->path);
		list = list->next;
	}*/
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

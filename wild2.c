// #include <string.h>
// #include <dirent.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <stdlib.h>

// typedef struct s_wild {
// 	char			*path;
// 	struct s_wild	*next;
// }	t_wild;

// t_wild	*print_dirs(const char *path, int recursive);

// /* OK */
// int		ft_compare(char *s1, char *s2)
// {
//     if (!*s1 && !*s2)
//         return (1);
//     else if (*s1 == *s2 && *s1 != '*')
//         return (ft_compare(s1 + 1, s2 + 1));
//     else if (*s1 == '*' && *s2 == '*')
//         return (ft_compare(s1 + 1, s2));
//     else if (*s2 == '*' && !*s1)
//         return (ft_compare(s1, s2 + 1));
//     else if (*s2 == '*' && *s2 && *s1)
//         return (ft_compare(s1, s2 + 1) || ft_compare(s1 + 1, s2));
//     else
//         return (0);
// }

/* OK */
// void	ft_strcpy(char *dst, const char *src)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (*src)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	dst[i] = '\0';
// }

/* OK */
// t_wild	*ft_lstnew_wild(void *content)
// {
// 	t_wild	*new_el;

// 	new_el = malloc(sizeof(t_wild));
// 	if (!new_el)
// 		return (NULL);
// 	new_el->path = content;
// 	new_el->next = NULL;
// 	return (new_el);
// }

/* OK */
// t_wild	*ft_lstlast_wild(t_wild *list)
// {
//     t_wild  *current;

//     if (!list)
//         return (NULL);
//     current = list;
//     while (current->next)
//         current = current->next;
//     return (current);
// }

/* OK */
// int		ft_strchr(char *str, char c)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == c)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

/* OK */
// int		ft_has_wildcards(char *str)
// {
//     if (ft_strchr(str, '\'') == 1 || ft_strchr(str, '\"') == 1)
// 		return (0);
// 	if (ft_strchr(str, '*'))
// 		return (1);
// 	return (0);
// }

// /* OK */
// char    *ft_delete_wild(char *dst, const char *src)
// {
//     int i;

//     i = 0;
//     while (src[i] != '*' && src[i] != '\0')
//         i++;
//     dst = malloc(sizeof(char) * (i + 1));
//     if (!dst)
//         return NULL;
//     i = 0;
//     while (src[i] != '*' && src[i] != '\0')
//     {
//         dst[i] = src[i];
//         i++;
//     }
//     dst[i] = '\0';
//     return (dst);
// }

/* OK */
// char    *ft_strcp(char *dst, const char *src)
// {
//     int i;

//     i = 0;
//     while (src[i] && src[i] != '\0')
//         i++;
//     dst = malloc(sizeof(char) * (i + 1));
//     if (!dst)
//         return NULL;
//     i = 0;
//     while (src[i] && src[i] != '\0')
//     {
//         dst[i] = src[i];
//         i++;
//     }
//     dst[i] = '\0';
//     return (dst);
// }

/* OK */
// int		ft_lstlen(t_wild *list, char *str)
// {
//     int i;

//     i = 0;
//     while (list)
//     {
//         if (ft_compare(list->path, str) == 1)
//             i++;
//         list = list->next;
//     }
//     return (i);
// }


// DIR		*open_dir(const char *path, size_t *path_len)
// {
// 	DIR		*dr;

// 	if (!path)
//         return NULL;
//     *path_len = strlen(path);  
//     if (!path || !*path_len || (*path_len > _POSIX_PATH_MAX))
//         return NULL;
//     dr = opendir(path);
//     if (dr == NULL)
//         return NULL;
// 	return (dr);
// }

// int		ft_recur_sa_mere(struct dirent *de, t_wild **tmp, char *full_name, int recursive)
// {
// 	struct stat		fstat;
// 	t_wild			*last;
// 	t_wild			*new;

// 	if (!((strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0))
// 			&& !(stat(full_name, &fstat) < 0) && (S_ISDIR(fstat.st_mode) || S_ISREG(fstat.st_mode)))
// 	{
// 		new = ft_lstnew_wild(full_name);
// 		if (!new)
// 			return (free(full_name), 1);
// 		if (!(*tmp))
// 			*tmp = new;
// 		else
// 		{
// 			last = ft_lstlast_wild(*tmp);
// 			last->next = new;
// 		}
// 		if (recursive)
// 		{
// 			last = ft_lstlast_wild(*tmp);
// 			last->next = print_dirs(full_name, 1);
// 		}
// 	}
// 	else
// 		free(full_name);
// 	return 0;
// }

// t_wild	*print_dirs(const char *path, int recursive)
// {
//     t_wild          *tmp;
// 	struct dirent	*de;
// 	DIR				*dr;
// 	size_t			path_len;
//     char            *full_name;

// 	dr = open_dir(path, &path_len);
// 	if (!dr)
// 		return NULL;
//     tmp = NULL;
//     while ((de = readdir(dr)) != NULL)
//     {
//         full_name = (char *)calloc((_POSIX_PATH_MAX + 1), sizeof(char));
//         if ((path_len + strlen(de->d_name) + 1) > _POSIX_PATH_MAX)
//             continue;
//         strcpy(full_name, path);
//         if (full_name[path_len - 1] != '/')
//             strcat(full_name, "/");
//         strcat(full_name, de->d_name);
// 		if (ft_recur_sa_mere(de, &tmp, full_name, recursive))
// 			return (closedir(dr), NULL);
//     }
//     closedir(dr);
//     return (tmp);
// }


/* OK */
// void	ft_wild(char **argv, char **argt, char ***tab)
// {
//     t_wild  *list;
// 	int     i;

// 	i = 0;
//     list = NULL;
// 	if (ft_has_wildcards(*argv) == 1)
//        list = print_dirs (*argt, 1);
// 	while (list)
// 	{
//         if (ft_compare(list->path, *argv) == 1)
//         {
//         	(*tab)[i] = ft_strcp((*tab)[i], list->path);
//         	i++;
//         }
// 		list = list->next;
// 	}
//     (*tab)[i] = NULL;
// }

// int		main(int argc, char **argv)
// {
//     char    *argt;
// 	char	**tab;
//     t_wild  *list;
//     int     i;
//     int     j;

//     i = 0;
// 	tab = NULL;
//     list = NULL;
//     argt = NULL;
// 	if (argc != 2)
// 		return (0);
//     if (ft_strchr(argv[1], '/'))
//         argt = ft_delete_wild(argt, argv[1]);
//     if (ft_has_wildcards(argv[1]) == 1)
//         list = print_dirs (argt, 1);
//     j = ft_lstlen(list, argv[1]);
//     tab = (char **)malloc(sizeof(char *) * (j + 1));
//     if (!tab)
//         return 1;
// 	ft_wild(&argv[1], &argt, &tab);
//     i = -1;
//     while (tab[++i])
//         printf("tab[%d] : %s\n", i, tab[i]);
// 	return 0;
// }

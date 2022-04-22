#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_wild {
	char			*path;
	struct s_wild	*next;
}	t_wild;

void	ft_strcpy(char *dst, const char *src)
{
	unsigned int	i;

	i = 0;
	while (*src)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

t_wild	*ft_lstnew(void *content)
{
	t_wild	*new_el;

	new_el = malloc(sizeof(t_wild));
	if (!new_el)
		return (NULL);
	new_el->path = content;
	new_el->next = NULL;
	return (new_el);
}

int print_dirs(const char *path, int recursive, t_wild *list)
{
	struct dirent	*de;
	DIR				*dr;
	size_t			path_len;
	struct stat		fstat;
	char			full_name[_POSIX_PATH_MAX + 1];

    if (!path)
        return -1;
    path_len = strlen(path);  
    if (!path || !path_len || (path_len > _POSIX_PATH_MAX))
        return -1;
    dr = opendir(path);
    if (dr == NULL)
        return -1;
    while ((de = readdir(dr)) != NULL)
    {
        if ((path_len + strlen(de->d_name) + 1) > _POSIX_PATH_MAX)
            continue;
        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
            strcat(full_name, "/");
        strcat(full_name, de->d_name);
        if ((strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0))
            continue;
        if (stat(full_name, &fstat) < 0)
            continue;
        if (S_ISDIR(fstat.st_mode) || S_ISREG(fstat.st_mode))
        {
			list->path = full_name;
            printf("%s C'est ca\n", full_name);
            if (recursive)
                print_dirs(full_name, 1, list);
        }
    }
    (void)closedir(dr);
    return 0;
}

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
	// char	**tab;
	t_wild	*list;

	list = ft_lstnew("NULL");
	// tab = NULL;
	if (argc != 2)
		return (0);
	print_dirs(argv[1], 1, list);
	while (list->next)
	{
		printf("list : %s\n", list->path);
		list = list->next;
	}
	return 0;
}

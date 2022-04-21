#include "../../includes/minishell.h"
#include <dirent.h>


int    test(char *s1, char *s2)
{
    if (!*s1 && !*s2)
        return (1);
    else if (*s1 == *s2 && *s1 != '*')
        return (match(s1 + 1, s2 + 1));
    else if (*s1 == '*' && *s2 == '*')
        return (match(s1 + 1, s2));
    else if (*s2 == '*' && !*s1)
        return (match(s1, s2 + 1));
    else if (*s2 == '*' && *s2 && *s1)
        return (match(s1, s2 + 1) || match(s1 + 1, s2));
    else
        return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strcpy1(char *dst, const char *src)
{
	char	*save;

	save = dst;
	while (src && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (save);
}

char	*ft_strdup1(const char *str)
{
	char	*cpy;
	size_t	len;

	len = ft_strlen(str);
	cpy = malloc(len + 1);
	if (!cpy)
		return (NULL);
	return (ft_strcpy1(cpy, str));
}

int	ft_iffirst(char *str, char c)
{
	if (str[0] == c)
		return (1);
	return (0);
}

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_has_wildcards(char *str)
{
	if (ft_strchr(str, '\'') == 1 || ft_strchr(str, '\"') == 1)
		return (0);
	if (ft_strchr(str, '*') && !ft_strchr(str, '/'))
		return (1);
	return (0);
}

int num_dirs(const char* path)
{
    int dir_count = 0;
    struct dirent* dent;
    DIR* srcdir = opendir(path);

    if (srcdir == NULL)
    {
        perror("opendir");
        return -1;
    }

    while((dent = readdir(srcdir)) != NULL)
    {
        struct stat st;

        if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
        {
            perror(dent->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode)) dir_count++;
    }
    closedir(srcdir);
    return dir_count;
}

void list_dir(char *path, int indent) {
  DIR *dir;
  struct dirent *entry;
  dir = opendir(path);
  
  if (dir == NULL) {
    printf("Failed to open directory.\n");
    return;
  }
  while ((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_DIR) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        printf("%c%s\n", indent2, '-', entry->d_name);
        char *new_path = (char *)malloc(strlen(path) + strlen(entry->d_name) + 1);
        sprintf(new_path, "%s/%s", path, entry->d_name);
        list_dir(new_path, indent + 1);
        free(new_path);
      }
      } else {
        printf("%c%s\n", indent2, '-', entry->d_name);
      }
    }
    closedir(dir);
}


int	ft_wildcards(char **tab, char *arg)
{
	struct dirent	*de;
  	DIR				*dr;
    int				i;
	int				j;
	// char			*pwd;
	// char			**bef;

	// pwd = ft_genv("PWD", info->env);
	dr = opendir(".");
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return (0);
    }
	i = 0;
	while ((de = readdir(dr)) != NULL)
		i++;
	closedir(dr);

	dr = opendir(".");
	j = 0;
	while ((de = readdir(dr)) != NULL)
	{
		tab = (char **)malloc(sizeof(char *) * (i + 1));
		if (!tab)
			return (0);
		if (ft_iffirst(de->d_name, '.') == 0)
		{
			tab[j]= ft_strdup1(de->d_name);
			printf("tab[%d] = %s\n\n", j, tab[j]);
			j++;
		}
	}
	closedir(dr);
	free(tab);
    return (1);
}

int main (int argc, char **argv)
{
	char	**tab;

	tab = NULL;
	if (argc != 2)
		return (0);
	if (ft_has_wildcards(argv[1]) != 0)
	{
		ft_wildcards(tab, argv[1]);
		return (1);
	}
	return (0);
}

/*Get the wildcard lists object

char    get_wildcard_files(char *wildcard, t_shell *shell)
{
    char            *pwd;
    DIR                *dir;
    struct dirent    *entry;
    t_automaton        *au;
    char            **lists;

    //lists = 0;
    au = automaton_wildcard_factory(wildcard);
    if (!au)
        return (0);
    //pwd = get_env(shell, "PWD");
    //dir = opendir(pwd);
    if (dir)
    {
        entry = readdir(dir);
        while (entry)
        {
            if (automaton_validator(au, entry->d_name))
                push_array(entry->d_name, &lists);
            entry = readdir(dir);
        }
    }
    closedir(dir);
    automaton_dispose(au);
    return (lists);
}

void    wildcard_cmds(t_shell *shell)
{
    t_cmd    *cmd;
    int        i;
    char    lists;

    cmd = shell->cmds;
    while (cmd)
    {
        i = -1;
        while (cmd->args && cmd->args[++i])
        {
            if (has_wildcard(cmd->args[i]))
            {
                lists = get_wildcard_files(cmd->args[i], shell);
                if (lists)
                {
                    insert_array(lists, cmd->args[i], &cmd->args);
                    remove_str_array(cmd->args[i], &cmd->args);
                    free_array(lists);
                }
            }
        }
        cmd = cmd->next;
    }
}
*/

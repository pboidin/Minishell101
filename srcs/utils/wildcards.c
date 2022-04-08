#include <minishell.h>

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
	if (ft_strchr(str, '*') && !ft_strchr(str, '/'))
		return (1);
	return (0);
}

char	**ft_wildcards(char *cmd, ??)
{
	struct dirent	*de;
  	DIR				*dr;
	char			*pwd;
	char			**ret;

	ret = NULL;
	pwd = get_env(cmd, "PWD");
	dr = opendir(pwd);
  
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }
	if (dr)
	{
		de = readdir(dr);
		while (de)
		{
			if ()
				-->
			de = readdir (dr);
		}
	}
    closedir(dr);
    return (ret);
}


//Get the wildcard lists object

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


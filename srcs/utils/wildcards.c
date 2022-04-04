#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

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

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

int main(int argc, char **argv)
{
	char	*cmd;
	struct	dirent *de;
	(void)	argc;
	int		i;

	DIR *dr = opendir(".");
	i = 0;
	if (dr == NULL)
	{
		printf("Could not open current directory");
		return (0);
	}
	cmd = argv[1];
	if (argv[1] == NULL)
		return (0);
	while (cmd[i])
	{
		write(1, &cmd[i], 1);
		i++;
	}
	write(1, "\n", 1);
	i = 0;
	if (cmd == NULL)
        return (0);
	if (ft_strchr(argv[1], '*') == 1)
	{
		while ((de = readdir(dr)) != NULL)
		{
			while (argv[1][i] == '*')
				i++;
			if (ft_strcmp(argv[1]))
			ft_putstr(de->d_name);
		}
    	closedir(dr);    
    	return 0;
	}
}
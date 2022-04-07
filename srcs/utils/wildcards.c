#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>

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

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strreturn(char *str, int i)
{
	char	*ret;

	printf("%i", i);
	ret = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (str == NULL || ret == NULL)
		return (NULL);
	while (str[i] != ' ' || str[i] != '\n' || str[i] != '\0')
	{
		write(1, "str = ", 6);
		write(1, &str[i], 1);
		write(1, "\n", 1);
		ret[i] = str[i];
		i++;
	}
	write(1, "Ici\n", 4);
	ret[i] = '\n';
	i = 0;
	while (ret[i])
	{
		write(1, &ret[i], 1);
		i++;
	}
	return (ret);
}

char	**ft_wildcards(char *cmd, char **ret)
{
	struct dirent *de;
  
    DIR *dr = opendir(".");
  
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }
	if (ft_strchr(cmd, '*') == 1)
	{
		while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);
		ret = 1;
	}
	else
		ret = 0;
    closedir(dr);
    return (ret);
}

int main(int argc, char **argv)
{
	char	*cmd;
	char	**ret;
	(void)	argc;

	cmd = argv[1];
	if (cmd == NULL)
		return (0);
	ret = ft_wildcards(cmd, ret);

	return (ret);
}

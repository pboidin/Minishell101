#include "minishell.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putendl_fd(char *str, int fd)
{
	char	nline;

	nline = '\n';
	write(fd, str, ft_strlen(str));
	write(fd, &nline, 1);
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
}

char	*ft_trim(char *cmd)
{
	int		i;
	int		j;
	int		z;
	char	*new_cmd;

	i = 0;
	skip_whitespaces(cmd, &i);
	if (!cmd || !cmd[i])
		return (NULL);
	j = i;
	while (cmd[j + 1])
		j++;
	while (j >= i
		&& ((cmd[i + j] >= '\t' && cmd[i + j] <= '\r') || cmd[i + j] == ' ' ))
		j--;
	new_cmd = (char *)malloc(sizeof(char) * (j - i + 2));
	if (!new_cmd)
		return (NULL);
	z = 0;
	while (i <= j)
		new_cmd[z++] = cmd[i++];
	new_cmd[z] = '\0';
	return (new_cmd);
}

void	skip_whitespaces(char *cmd, int *i)
{
	if (!cmd)
		return ;
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
}

#include "../includes/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void	ft_print(void)
{
	char	*values;
	int		i;
	t_env	*env;

	env = *g_info.env;
	while (env)
	{
		i = 0;
		values = (char *)env->value;
		while (values[i] && values[i] != '=')
			i++;
		if (values[i] == '=' && values[i + 1])
		{
			write(1, env->value, ft_strlen((char *)env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
}

int	ft_env(char **tab)
{
	if (!g_info.env)
		return (0);
	if (tab[1] == NULL)
		ft_print();
	else
	{
		write(STDERR_FILENO, "env: ", 5);
		write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	}
	return (0);
}

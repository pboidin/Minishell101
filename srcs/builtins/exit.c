#include "minishell.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	rslt;
	int	neg;

	i = 0;
	rslt = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		rslt = rslt * 10 + (str[i] - 48);
		i++;
	}
	return (rslt * neg);
}

static int	ft_exit_code(char *const *tab, int i)
{
	if (tab && tab[0] && tab[1] && tab[2])
	{
		if (g_info.is_child == FALSE)
			write(STDERR_FILENO, "exit\n", 5);
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		return (1);
	}
	while (tab[1] && tab[1][i])
	{
		if (!ft_isdigit(tab[1][i++]))
		{
			if (g_info.is_child == FALSE)
				write(STDERR_FILENO, "exit\n", 5);
			write(STDERR_FILENO, "exit: ", 6);
			write(STDERR_FILENO, tab[1], ft_strlen(tab[1]));
			write(STDERR_FILENO, ": numeric argument required\n", 28);
			g_info.status = 2;
			ft_lstclear(g_info.env, free);
			free(g_info.env);
			exit(2);
		}
	}
	return (0);
}

int	ft_exit(char **tab)
{
	int	exit_code;
	int	i;

	i = 0;
	if (tab[1] && (tab[1][i] == '+' || tab[1][i] == '-'))
		i++;
	if (ft_exit_code(tab, i) == 1)
		return (1);
	if (tab[1])
	{
		exit_code = ft_atoi(tab[1]);
		g_info.status = (unsigned char)exit_code;
		ft_lstclear(g_info.env, free);
		free(g_info.env);
		if (g_info.is_child == FALSE)
			write(STDERR_FILENO, "exit\n", 5);
		exit((unsigned char)exit_code);
	}
	ft_lstclear(g_info.env, free);
	free(g_info.env);
	if (g_info.is_child == FALSE)
		write(STDERR_FILENO, "exit\n", 5);
	exit(g_info.status);
	return (0);
}

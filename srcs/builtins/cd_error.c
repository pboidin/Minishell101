#include "minishell.h"

void	ft_env_set(t_env *env, t_info *info)
{
	ft_env_loc(env, info);
}

int	ft_print_err(const char *dir)
{
	if (dir)
	{
		write(STDERR_FILENO, "cd: ", 4);
		write(STDERR_FILENO, dir, ft_strlen(dir));
		write(STDERR_FILENO, ": no such file or directory:\n", 29);
	}
	else
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
	return (1);
}

int	ft_ret_home(t_info *info)
{
	if (ft_go_to_home(info) == 1)
		return (1);
	return (0);
}
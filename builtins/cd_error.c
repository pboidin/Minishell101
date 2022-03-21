#include "../includes/minishell.c"

void	ft_env_set(t_env *env)
{
	ft_env_loc(env);
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

int	ft_return_home(void)
{
	if (ft_go_to_home() == 1)
		return (1);
	return (0);
}
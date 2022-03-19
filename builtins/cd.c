#include "../includes/minishell.h"

static int	ft_go_to_pwd(t_env *env)
{
	char	*tmp;
	char	*old;
	char	*pwd;

	tmp = NULL;
	pwd = NULL;
	old = ft_genv("OLDPWD");
	if (!old)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		return (1);
	}
	if (chdir(pwd) == -1)
	{
		write(STDERR_FILENO, "cd: ", 4);
		write(STDERR_FILENO, old, ft_strlen(old));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		return (1);
	}
	free(old);


}

int	ft_ch_dir(char **dir)
{
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	tmp = NULL;

	if (dir[1] && ft_strncmp(dir[1], "-", 1) == 0)
		return (ft_go_to_pwd(g_info.env));
	ft_set_old(g_info.env, pwd, tmp);
	if (!dir[1] || (!ft_strncmp(dir[1], "~\0", 2) || !ft_strncmp(dir[1], "~/", 2)))
	{
		if (!dir[1] || !ft_strncmp(dir[1], "~\0", 2))
		{
			
		}
	}
}
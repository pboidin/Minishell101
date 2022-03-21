#include "../includes/minishell.h"

static int	ft_go_to_dir(const char *dir)
{
	char	*tmp;
	char	*aux;
	char	*home;

	aux = ft_substr(dir, 1, ft_strlen(dir) - 1);
	home = ft_genv("HOME");
	if (!home)
	{
		free(aux);
		return (1);
	}
	aux = // Créer une fonction Réalloc qui va prendre en argmts aux, la taille de dir + la taille de home +1;
	tmp = ft_strjoin(home, aux);
	if (chdir(tmp) == -1)
	{
		ft_print_err(tmp);
		free(home);
		free(aux);
		free(tmp);
		return (1);
	}
	free(home);
	free(aux);
	free(tmp);
	return (0);
}

void	ft_upd_env(t_env **env)
{
	char	*path;

	path = malloc(sizeof(char) * (PATH_MAX + 1));
	if (!path)
		return ;
	if (getcwd(path, PATH_MAX) == NULL)
	{
		free(path);
		path = ft_genv("PWD");
		if (path)
			return ;
	}
	free((*env)->value);
	(*env)->value = ft_strjoin("PWD=", path);
	free(path);
}

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
	ft_set_old(env, pwd, tmp);
	ft_env_loc(env);
	return (0);
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
			if (ft_ret_home() == 1)
				return (1);
			return (0);
		}
		else if (ft_go_to_dir(dir[1]) == 1)
			return (1);
	}
	else if (dir[1][0] != '\0' && chdir(dir[1] == -1))
		return (ft_print_err(dir[1]));
	else
		ft_env_set(*g_info.env);
	return (0);
}
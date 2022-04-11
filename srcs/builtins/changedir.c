# include <errno.h>
# include "minishell.h"

static int	ft_goto_home(char *str, t_env *env)
{
	if (ft_has_env("HOME", env))
	{
		if (chdir(ft_getvalue("HOME", env)) < 0)
		{
			ft_error_print(errno, str);
			return (1);
		}
	}
	else
	{
		if (str && ft_strcmp(str, "~"))
		{
			if (ft_has_env("$HOME", env))
			{
				if (chdir(ft_getvalue("$HOME", env)) < 0)
				{
					ft_error_print(errno, str);
					return (1);
				}
				return (0);
			}
		}
		return (ft_home_err_prt());
	}
	return (0);
}

static int	ft_cd_absolute(char *str)
{
	if (chdir(str) < 0)
	{
		ft_error_print(errno, str);
		return (1);
	}
	return (0);
}

static int	ft_cd_relative(char *str, t_env *env)
{
	int		res;
	char	*tmp;

	res = 0;
	if (*str == '~')
	{
		if (!ft_has_env("$HOME", env))
			return (ft_home_err_prt());
		if (str[1] == '/')
			tmp = ft_strjoin(ft_getvalue(env, "$HOME"), str + 1);
		else
			tmp = ft_strdup(str);
		if (chdir(tmp) < 0)
		{
			ft_error_print(errno, str);
			res = 1;
		}
		free(tmp);
	}
	else if (chdir(str) < 0)
	{
		ft_error_print(errno, str);
		res = 1;
	}
	return (res);
}

void	ft_env_pwd_upd(t_env *env)
{
	char	*pwd;
	char	*oldpwd;

	if (!ft_has_env("PWD", env))
		return ;
	oldpwd = ft_strdup(ft_getvalue("PWD", env));
	pwd = getcwd(NULL, 0);
	ft_modify_env("PWD", pwd, env);
	free(pwd);
	if (ft_has_env("OLDPWD", env))
		ft_modify_env("OLDPWD", oldpwd, env);
	free (oldpwd);
}

int	ft_cd(char **dir, t_info *info)
{
	int	res;

	if (!dir[1] || ft_strcmp(dir[1], "") || ft_strcmp(dir[1], "~"))
		res = ft_go_home(dir[1], info->env);
	else if (*(dir[1]) == '/')
		res = ft_cd_absolute(dir[1]);
	else
		res = ft_cd_relative(dir[1], info->env);
	if (res == 0)
		ft_env_pwd_upd(info->env);
	return (res);
}
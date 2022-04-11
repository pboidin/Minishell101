# include <errno.h>
# include <stdio.h>
# include "minishell.h"

void	ft_modify_env(char *name, char *value, t_env *env)
{
	char	*tmp;

	while (env)
	{
		if (ft_strcmp(name, env->name))
		{
			if (!value)
				return ;
			tmp = env->value;
			env->value = ft_strdup(value);
			free (tmp);
			return ;
		}
		env = env->next;
	}
}

int	ft_home_err_prt(void)
{
	int	exit_status;

	ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
	exit_status = 1;
	return (exit_status);
}

void	ft_error_print(int err, char *str)
{
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(stderror(err), STDERR_FILENO);
}

char	*ft_getvalue(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	ft_has_env(char *str, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(str, env->name))
			return (1);
		env = env->next;
	}
	return (0);
}
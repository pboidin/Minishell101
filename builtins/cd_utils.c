#include "../includes/minishell.h"

void	ft_set_val(t_env **env, const char *val, int c)
{
	if (c == 0)
	{
		free((*env)->value);
		(*env)->value = (void *)ft_strdup(val);
	}
	else
		ft_lstadd_back(g_info.env, ft_lstnew((void *) ft_strdup(val)));
}

int	ft_set_old(t_env *env, char *pwd, char *val)
{
	int		ret;
	t_env	*tmp;

	tmp = env;
	pwd = ft_genv("PWD");
	
	if (tmp = NULL)
	{
		free(pwd);
		return (1);
	}
	if (pwd == NULL)
	{
		pwd = (char *)malloc(sizeof(char) * (PATH_MAX + 1));
		if (!pwd || getcwd(pwd, PATH_MAX) == NULL)
		{
			free(pwd);
			return (1);
		}
	}
	val = ft_strjoin("OLDPWD=", pwd);
	ft_set_val(&tmp, val, ret);
	free(pwd);
	free(val);
	return (0);
}
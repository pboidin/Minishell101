#include "minishell.h"

#ifndef PATH_MAX
# define PATH_MAX 256
#endif

char	*ft_genv(const char *path)
{
	int		path_size;
	char	*env;
	char	*ret;
	t_env	*tmp;

	tmp = *g_info.env;
	env = ft_strjoin(path, "=");
	path_size = ft_strlen(env);
	while (tmp)
	{
		if (ft_strncmp(env, (char *)tmp->value, path_size) == 0)
		{
			ret = ft_substr((char *)tmp->value, path_size, ft_strlen((char *)tmp->value) - path_size);
			free(env);
			return (ret);
		}
		tmp = tmp->next;
	}
	free(env);
	return (NULL);
}

static void	ft_print_pwd(const char *pwd)
{
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
}

int	ft_working_dir(char **n)
{
	char	*pwd;

	pwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!pwd)
		return (1);
	(void)n;
	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		free(pwd);
		pwd = NULL;
	}
	if (pwd == NULL)
		pwd = ft_genv("PWD");
	if (pwd == NULL)
		return (1);
	ft_print_pwd(pwd);
	free(pwd);
	return (0);
}

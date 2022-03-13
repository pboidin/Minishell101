#include "../includes/minishell.h"

static void	ft_print_err(const char *new_env)
{
	write(STDERR_FILENO, "export: ", 8);
	write(STDERR_FILENO, new_env, ft_strlen(new_env));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "not a valid identifier\n", 23);
}

int	ft_error(char *new_env)
{
	int	i;

	i = 0;
	if ((new_env[0] >= 'A' && new_env[0] <= 'Z')
		|| (new_env[0] >= 'a' && new_env[0] <= 'z')
		|| new_env[0] == '_')
	{
		while (new_env[++i] != '=')
		{
			if (ft_isalnum(new_env[i]) == 0
				&& new_env[i] != '_' && new_env[i] != '=')
			{
				ft_print_err(new_env);
				return (1);
			}
		}
		return (0);
	}
	else
		ft_print_err(new_env);
	return (1);
}

void	ft_add_env(const char *new_env, int i)
{
	if (ft_error(new_env[i]) == 0)
		ft_lstadd_back(g_info.env,
			ft_lstnew((void *)ft_strdup(new_env[i])));
}

void	ft_export_var(char **new_env, char *tmp, char *env, char *env2)
{
	int	i;

	i = 0;
	while (new_env[++i])
	{
		tmp = ft_substr(new_env[i], 0, ft_lenvar(new_env[i]));
		env = ft_genv(tmp);
		if (!env)
		{
			if (new_env[i][ft_lenvar(new_env[i])] != '=')
			{
				env2 = ft_strjoin(new_env[i], "=");
				free(new_env[i]);
				new_env[i] = env2;
			}
			ft_add_env(new_env, i);
			free(tmp);
		}
		else
		{
			free(env);
	//		ft_update_var(tmp, new_env[i]); // need to write this function.
		}
	}
}

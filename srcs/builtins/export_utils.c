/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:37 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:47:39 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_var(char *var, char *name_var, t_info *info)
{
	char	*content;
	char	*tmp;
	t_env	*env;

	env = info->env;
	tmp = ft_strjoin(name_var, "=");
	free(name_var);
	content = ft_substr(var, ft_lenvar(var) + 1,
			ft_strlen(var) - ft_strlen(tmp));
	while (env != NULL)
	{
		if (ft_strncmp(tmp, (char *)env->value, ft_strlen(tmp)) == 0)
			break ;
		env = env->next;
	}
	free(env->value);
	env->value = ft_strjoin(tmp, content);
	free(tmp);
	free(content);
}

static void	ft_print_err_export(const char *new_env)
{
	write(STDERR_FILENO, "export: ", 8);
	write(STDERR_FILENO, new_env, ft_strlen(new_env));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "not a valid identifier\n", 23);
}

int	ft_error_export(char *new_env)
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
				ft_print_err_export(new_env);
				return (1);
			}
		}
		return (0);
	}
	else
		ft_print_err_export(new_env);
	return (1);
}

void	ft_add_env(char *const *new_env, int i, t_info *info)
{
	if (ft_error_export(new_env[i]) == 0)
		ft_lstadd_back(&info->env,
			ft_lstnew((void *)ft_strdup(new_env[i])));
}

void	ft_export_var(char **new_env, char *env, char *env2, t_info *info)
{
	int		i;
	char	*tmp;

	i = 0;
	while (new_env[++i])
	{
		tmp = ft_substr(new_env[i], 0, ft_lenvar(new_env[i]));
		env = ft_genv(tmp, info);
		if (!env)
		{
			if (new_env[i][ft_lenvar(new_env[i])] != '=')
			{
				env2 = ft_strjoin(new_env[i], "=");
				free(new_env[i]);
				new_env[i] = env2;
			}
			ft_add_env(new_env, i, info);
			free(tmp);
		}
		else
		{
			free(env);
			ft_update_var(tmp, new_env[i], info);
		}
	}
}

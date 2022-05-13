/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:47:37 by piboidin          #+#    #+#             */
/*   Updated: 2022/05/13 16:24:16 by bdetune          ###   ########.fr       */
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
		while (new_env[++i] && new_env[i] != '=')
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

void	ft_add_env(char *const *new_env, int i, t_info *info, char *tmp[2])
{
	if (ft_error_export(new_env[i]) == 0)
		ft_lstadd_back(&info->env,
			ft_lstnew_export(tmp));
}

void	export_lcl_to_env(t_var *lcl_var, t_info *info)
{
	char	*tmp[2];
	t_var	*prev;
	t_var	*next;

	tmp[0] = lcl_var->name;
	tmp[1] = lcl_var->value;
	next = lcl_var->next;
	if (info->local_var == lcl_var)
	{
		free(lcl_var);
		info->local_var = next;
	}
	else
	{
		prev = info->local_var;
		while (prev->next != lcl_var)
			prev = prev->next;
		free(lcl_var);
		prev->next = next;
	}
	ft_lstadd_back(&info->env, ft_lstnew_export(tmp));
}

void	ft_suite_export(char **new_env, t_info *info, char *tmp[2], int i)
{
	t_var	*lcl_var;

	tmp[0] = get_var_name(new_env[i], info);
	lcl_var = find_lcl_var(info, tmp[0]);
	if (!lcl_var)
	{
		tmp[1] = NULL;
		ft_add_env(new_env, i, info, tmp);
	}
	else
	{
		free(tmp[0]);
		export_lcl_to_env(lcl_var, info);
	}
}

void	ft_export_var(char **new_env, t_info *info)
{
	int		i;
	char	*tmp[2];
	t_env	*target;

	i = 0;
	info->status = 0;
	while (new_env[++i])
	{
		if (is_assignation(new_env[i]))
		{
			tmp[0] = get_var_name(new_env[i], info);
			tmp[1] = get_var_val(new_env[i], info, tmp);
			target = find_env_var(info, tmp[0]);
			if (!target)
				ft_add_env(new_env, i, info, tmp);
			else
				update_env_var(tmp, target);
			info->status = 0;
		}
		else if (ft_error_export(new_env[i]))
			info->status = 1;
		else
			ft_suite_export(new_env, info, tmp, i);
		// {
		// 	tmp[0] = get_var_name(new_env[i], info);
		// 	lcl_var = find_lcl_var(info, tmp[0]);
		// 	if (!lcl_var)
		// 	{
		// 		tmp[1] = NULL;
		// 		ft_add_env(new_env, i, info, tmp);
		// 	}
		// 	else
		// 	{
		// 		free(tmp[0]);
		// 		export_lcl_to_env(lcl_var, info);
		// 	}
		// }
	}
}

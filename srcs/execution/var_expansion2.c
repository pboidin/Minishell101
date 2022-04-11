#include "minishell.h"

char	*find_var(char *var, t_info *info)
{
	char	*value;
	t_env	*current_env;
	t_var	*current_var;

	current_env = info->env;
	while (current_env)
	{
		if (!ft_strcmp(&var[1], current_env->name))
		{
			value = ft_strdup(current_env->value);
			return (value);
		}
		current_env = current_env->next;
	}
	current_var = info->local_var;
	while (current_var)
	{
		if (!ft_strcmp(&var[1], current_var->name))
		{
			value = ft_strdup(current_var->value);
			return (value);
		}
		current_var = current_var->next;
	}
	return ((char *)ft_calloc(1, sizeof(char)));
}

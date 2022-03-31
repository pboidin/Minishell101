/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:48:15 by piboidin          #+#    #+#             */
/*   Updated: 2022/03/31 14:48:18 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_err_uns(const char *new_env)
{
	write(STDERR_FILENO, "unset: ", 7);
	write(STDERR_FILENO, new_env, ft_strlen(new_env));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "not a valid identifier\n", 23);
}

int	ft_unset_handle(char *new_env)
{
	int	i;

	i = 0;
	if ((new_env[0] >= 'A' && new_env[0] <= 'Z')
		|| (new_env[0] >= 'a' && new_env[0] <= 'z')
		|| (new_env[0] == '_'))
	{
		while (new_env[++i])
		{
			if (ft_isalnum(new_env[i]) == 0 && new_env[i] != '_')
			{
				ft_print_err_uns(new_env);
				return (1);
			}
		}
		return (0);
	}
	else
		ft_print_err_uns(new_env);
	return (1);
}

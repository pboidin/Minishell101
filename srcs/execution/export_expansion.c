/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 17:11:01 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 17:32:01 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_block	**create_export_t_block_tab(void)
{
	t_block	**t_block_tab;

	t_block_tab = ft_calloc(2, sizeof(t_block *));
	if (!t_block_tab)
		return (NULL);
	t_block_tab[0] = ft_calloc(2, sizeof(t_block));
	if (!t_block_tab[0])
		return (free(t_block_tab), NULL);
	t_block_tab[0][0].str = ft_strdup("export");
	if (!t_block_tab)
		return (free(t_block_tab[0]), free(t_block_tab), NULL);
	return (t_block_tab);
}

t_block	**expand_export_var(t_cmd *cmd, t_info *info)
{
	size_t	nb_args;
	t_block	**t_block_tab;
	t_block	**ret;

	t_block_tab = create_export_t_block_tab();
	if (!t_block_tab)
		return (sys_call_error(info), NULL);
	nb_args = 1;
	while (cmd->cmd_args[nb_args])
	{
		if (is_assignation(cmd->cmd_args[nb_args]))
			ret = add_args_word(cmd->cmd_args[nb_args], info, 2);
		else
			ret = add_args_word(cmd->cmd_args[nb_args], info, 1);
		if (!ret)
			return (sys_call_error(info), NULL);
		t_block_tab = add_block_to_tab(t_block_tab, ret);
		if (!t_block_tab)
			return (sys_call_error(info), NULL);
		nb_args++;
	}
	return (t_block_tab);
}

int	export_expansion(t_info *info, t_cmd *cmd)
{
	size_t	i;
	t_block	**t_block_tab;
	char	**new_args;

	t_block_tab = expand_export_var(cmd, info);
	if (!t_block_tab)
		return (sys_call_error(info), 1);
	i = 0;
	while (t_block_tab[i])
	{
		if (is_empty_var(t_block_tab[i]))
		{
			free_t_block(t_block_tab[i]);
			t_block_tab[i] = NULL;
			move_t_block_tab_upward(t_block_tab, (i + 1), 1);
			i--;
		}
		i++;
	}
	new_args = t_block_tab_to_char_tab(t_block_tab);
	if (!new_args)
		return (free_t_block_tab(t_block_tab), sys_call_error(info), 1);
	free_char_tab(cmd->cmd_args);
	cmd->cmd_args = new_args;
	return (free_t_block_tab(t_block_tab), 0);
}

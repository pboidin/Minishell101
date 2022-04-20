/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_final_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 11:34:17 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 11:40:25 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block	**add_block_to_tab(t_block **old_tab, t_block **to_add)
{
	size_t	i;
	size_t	j;
	t_block	**new_tab;

	new_tab = (t_block **)ft_calloc((t_block_tab_size(old_tab) \
			+ t_block_tab_size(to_add) + 1), sizeof(t_block *));
	if (!new_tab)
		return (free_t_block_tab(old_tab), free_t_block_tab(to_add), NULL);
	i = 0;
	while (old_tab && old_tab[i])
	{
		new_tab[i] = old_tab[i];
		i++;
	}
	j = 0;
	while (to_add && to_add[j])
	{
		new_tab[i + j] = to_add[j];
		j++;
	}
	free(old_tab);
	free(to_add);
	return (new_tab);
}

t_block	**expand_cmd_var(t_cmd *cmd, t_info *info)
{
	size_t	nb_args;
	t_block	**t_block_tab;
	t_block	**ret;

	t_block_tab = NULL;
	nb_args = 0;
	while (cmd->cmd_args[nb_args])
	{
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

int	get_final_cmd(t_cmd *cmd, t_info *info)
{
	size_t	i;
	t_block	**t_block_tab;
	char	**new_args;

	t_block_tab = expand_cmd_var(cmd, info);
	if (!t_block_tab)
		return (1);
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
		return (free_t_block_tab(t_block_tab), 1);
	free_char_tab(cmd->cmd_args);
	cmd->cmd_args = new_args;
	return (free_t_block_tab(t_block_tab), 0);
}

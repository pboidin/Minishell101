/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:21:10 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/19 12:22:07 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	logical_controller(t_info *info, t_cmd *cmd)
{
	size_t	i;

	i = 0;
	while (cmd->sub_cmd[i])
	{
		if (i == 0)
			general_controller(info, cmd->sub_cmd[i]);
		else
		{
			if (cmd->sub_cmd[i]->prev_delim == AND && !info->status)
				general_controller(info, cmd->sub_cmd[i]);
			else if (cmd->sub_cmd[i]->prev_delim == OR && info->status)
				general_controller(info, cmd->sub_cmd[i]);
		}
		i++;
	}
}

static void	pipe_controller(t_info *info, t_cmd *cmd)
{
	int		fd[3];
	size_t	i;
	int		ret;

	i = 0;
	while (cmd->pipe[i])
	{
		if (cmd->pipe[i + 1] && pipe(fd))
				sys_call_error(info);
		ret = fork();
		if (ret == -1)
			sys_call_error(info);
		else if (!ret)
			pipe_child(info, cmd, i, fd);
		else
		{
			if (add_pid(info, ret))
				sys_call_error(info);
			fd[2] = pipe_parent(cmd, i, fd);
		}
		i++;
	}
	get_exit_status(info);
}

char	**replace_var(t_block *words, size_t i, t_info *info)
{
	char	**var_val;
	char	*var_found;

	if (ft_strlen(words[i].str) == 1 || words[i].str[1] == '?')
	{
		var_val = (char **)ft_calloc(2, sizeof(char *));
		if (!var_val)
			return (NULL);
		if (words[i].str[1] == '?')
			var_val[0] = ft_itoa(info->status);
		else if (words[i + 1].str && (words[i + 1].str[0] == '"' || words[i + 1].str[0] == 39))
			var_val[0] = (char *)ft_calloc(1, sizeof(char));
		else
			var_val[0] = ft_strdup(words[i].str);
		if (!var_val[0])
			return (free(var_val), NULL);
	}
	else
	{
		var_found = find_var(words[i].str, info);
		if (!var_found)
			return (NULL);
		if (!ft_strlen(var_found))
		{
			var_val = (char **)ft_calloc(2, sizeof(char *));
			if (!var_val)
				return (free(var_found), NULL);
			var_val[0] = var_found; 
		}
		else
		{
			var_val = ft_split_charset(var_found, "\t\n\r\v\f ");
			free(var_found);
			if (!var_val)
				return (NULL);
		}
	}
	return (var_val);
}

void	fork_controller(t_info *info, t_cmd *cmd)
{
	int	ret;

	ret = fork();
	if (ret == -1)
		sys_call_error(info);
	if (!ret)
		fork_child(info, cmd);
	else
	{
		if (add_pid(info, ret))
			sys_call_error(info);
		get_exit_status(info);
	}
}

void	print_t_block_tab(t_block **tab)
{
	size_t	i;
	size_t	j;

	if (!tab)
		return ;
	printf("After variable expansion:\n");
	j = 0;
	while (tab[j])
	{
		i = 0;
		while (tab[j][i].str)
		{
			printf("%s", tab[j][i].str);
			i++;
		}
		printf("\n");
		j++;
	}
	printf("-------------------------------------------\n");
}

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
	print_t_block_tab(t_block_tab);
	new_args = t_block_tab_to_char_tab(t_block_tab);
	free_t_block_tab(t_block_tab);
	if (!new_args)
		return (1);
	free_char_tab(cmd->cmd_args);
	cmd->cmd_args = new_args;
	return (0);
}

void	simple_controller(t_info *info, t_cmd *cmd)
{
	if (handle_redirections(cmd, info))
	{
		info->status = 1;
		return ;
	}
	if (!cmd->cmd_args[0])
	{
		info->status = 0;
		return ;
	}
	if (is_assignation(cmd->cmd_args[0]))
	{
		handle_assignation(cmd, info);
		return ;
	}
	if (get_final_cmd(cmd, info))
	{
		info->status = 1;
		return ;
	}
	if (!cmd->cmd_args[0])
	{
		info->status = 0;
		return ;
	}
	if (ft_blt(cmd) == 0)
	{
		ft_blti(info, cmd);
	    return ;
	}
	simple_cmd_child(info, cmd);
}

void	general_controller(t_info *info, t_cmd *cmd)
{
	if (g_signal > 0)
	{
		info->status = g_signal;
		return ;
	}
	if (cmd->sub_cmd)
		logical_controller(info, cmd);
	else if (cmd->pipe)
		pipe_controller(info, cmd);
	else if (cmd->fork)
		fork_controller(info, cmd);
	else
		simple_controller(info, cmd);
}

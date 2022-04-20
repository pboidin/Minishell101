/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 11:35:34 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/20 11:38:27 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_redirections(t_cmd *cmd)
{
	if (cmd->in)
	{
		close(cmd->in->fd);
		cmd->in->fd = -1;
	}
	if (cmd->out)
	{
		close(cmd->out->fd);
		cmd->out->fd = -1;
	}
}

char	*get_var_name(char *str, t_info *info)
{
	size_t	i;
	char	*var_name;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	str[i] = '\0';
	var_name = ft_strdup(str);
	if (!var_name)
		return (sys_call_error(info), NULL);
	str[i] = '=';
	return (var_name);
}

char	*get_var_val(char *str, t_info *info, char *var[2])
{
	size_t	i;
	char	*var_value;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	var_value = ft_strdup(&str[i]);
	if (!var_value)
		return (free(var[0]), sys_call_error(info), NULL);
	return (var_value);
}

void	assign(t_cmd *cmd, t_info *info)
{
	size_t	i;
	char	*var[2];
	t_env	*env_var;
	t_var	*lcl_var;

	i = 0;
	while (cmd->cmd_args[i])
	{
		var[0] = get_var_name(cmd->cmd_args[i], info);
		var[1] = get_var_val(cmd->cmd_args[i], info, var);
		env_var = find_env_var(info, var[0]);
		lcl_var = find_lcl_var(info, var[0]);
		if (env_var)
			update_env_var(var, env_var);
		else if (lcl_var)
			update_lcl_var(var, lcl_var);
		else
			add_lcl_var(info, var);
		i++;
	}
}

int	handle_assignation(t_cmd *cmd, t_info *info)
{
	size_t	i;
	t_block	**ret;
	t_block	**new_block;
	char	**new_args;

	i = 0;
	close_redirections(cmd);
	ret = NULL;
	while (cmd->cmd_args[i])
	{
		new_block = add_args_word(cmd->cmd_args[i], info, 2);
		if (!new_block)
			return (sys_call_error(info), 1);
		ret = add_block_to_tab(ret, new_block);
		if (!ret)
			return (sys_call_error(info), 1);
		i++;
	}
	new_args = t_block_tab_to_char_tab(ret);
	free_t_block_tab(ret);
	if (!new_args)
		return (sys_call_error(info), 1);
	free_char_tab(cmd->cmd_args);
	cmd->cmd_args = new_args;
	return (assign(cmd, info), 0);
}

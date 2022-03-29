#include "minishell.h"

void	ft_blti(t_info *info, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd_args[0], "echo", ft_strlen(cmd->cmd_args[0])) != 0)
		info->status = ft_echo(cmd->cmd_args);
	// else if (ft_strncmp(cmd->cmd_args[0], "cd", ft_strlen(cmd->cmd_args[0])) != 0)
	// 	info->status = ft_ch_dir(cmd->cmd_args);
	// else if (ft_strncmp(cmd->cmd_args[0], "env", ft_strlen(cmd->cmd_args[0])) != 0)
	// 	info->status = ft_env(cmd->cmd_args);
	// else if (ft_strncmp(cmd->cmd_args[0], "exit", ft_strlen(cmd->cmd_args[0])) != 0)
	// 	info->status = ft_exit(cmd->cmd_args);
	// else if (ft_strncmp(cmd->cmd_args[0], "export", ft_strlen(cmd->cmd_args[0])) != 0)
	// 	info->status = ft_export(cmd->cmd_args);
	else if (ft_strncmp(cmd->cmd_args[0], "pwd", ft_strlen(cmd->cmd_args[0])) != 0)
		info->status = ft_working_dir(cmd->cmd_args, info);
	// else if (ft_strncmp(cmd->cmd_args[0], "unset", ft_strlen(cmd->cmd_args[0])) != 0)
	// 	info->status = ft_unset(cmd->cmd_args);
}
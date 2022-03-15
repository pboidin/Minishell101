//gcc main.c -lreadline

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

char	*get_str(char *line, char sep, size_t *index)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = *index;
	j = 0;
	while (line[i + j] && line[i + j] != sep)
		j++;
	str = (char *)malloc(sizeof(char) * (j + 1));
	if (!str)
		return (NULL);
	j = 0;
	while (line[i + j] && line[i + j] != sep)
	{
		str[j] = line[i + j];
		j++;
	}
	*index += j + 1;
	return (str);
}

int	add_to_env(t_info *info, char *line)
{
	size_t	index;
	t_env	*new_var;

	index = 0;
	new_var = (t_env *)malloc(sizeof(t_env));
	if (!new_var)
		return (1);
	new_var->name = get_str(line, '=', &index);
	if (!new_var->name)
		return (free(new_var), 1);
	new_var->value = get_str(line, '\0', &index);
	if (!new_var->value)
		return (free(new_var->name), free(new_var), 1);
	new_var->next = info->env;
	info->env = new_var;
	return (0);
}

void	free_env(t_info *info)
{
	t_env	*current;
	t_env	*next;

	current = info->env;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	info->env = NULL;
}

void	free_var(t_info *info)
{
	t_var	*current;
	t_var	*next;

	current = info->local_var;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	info->local_var = NULL;
}

void	free_running_processes(t_info *info)
{
	t_pid	*current;
	t_pid	*next;

	current = info->running_processes;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	info->running_processes = NULL;
}

/*
void	free_cmd(t_info *info)
{
	size_t	i;

	if (!info->cmd)
		return ;
	i = 0;
	while (info->cmd[i])
	{
		free(info->cmd[i]->cmd);
		free(info->cmd[i]);
		i++;
	}
}
*/
void	free_info(t_info *info)
{
	free_env(info);
	free_var(info);
	free_running_processes(info);
//	free_cmd(info);
}

int	create_info(t_info *info, char **envp, char *name)
{
	size_t	i;

	info->prog_name = name;
	info->local_var = NULL;
	info->running_processes = NULL;
	info->last_ret = 0;
	info->env = NULL;
	if (envp)
	{
		i = 0;
		while (envp[i])
			i++;
		while (i--)
		{
			if (add_to_env(info, envp[i]))
				return (free_env(info), 1);
		}
	}
	return (0);
}

char *print_delim(int delim)
{
	 return (delim == 0 ? ("") : (delim == 1 ? ("|") : (delim == 2 ? ("&&") : ("||"))));
}

void	execute_command(t_cmd *cmd)
{
	long long	i;

	if (cmd->fork)
		printf("(");
	i = 0;
	if (cmd->sub_cmd)
	{
		while (cmd->sub_cmd[i])
		{
			execute_command(cmd->sub_cmd[i]);
			printf("%s", print_delim(cmd->sub_cmd[i]->next_delim));
			i++;
		}
	}
	else if (cmd->pipe)
	{
		while (cmd->pipe[i])
		{
			execute_command(cmd->pipe[i]);
			if (cmd->pipe[i + 1])
				printf("|");
			i++;
		}
	}
	else
		printf("%s", cmd->cmd_name);
	if (cmd->fork)
		printf(")");
}

char	*ft_trim(char *cmd)
{
	size_t	i;
	size_t	j;
	size_t	z;
	char	*new_cmd;

	i = 0;
	while (cmd[i] && ((cmd[i] >= '\t' && cmd[i] <= '\r') || cmd[i] == ' ' ))
		i++;
	if (!cmd[i])
		return (NULL);
	j = i;
	while (cmd[j])
		j++;
	j--;
	while (j >= i
		&& ((cmd[i + j] >= '\t' && cmd[i + j] <= '\r') || cmd[i + j] == ' ' ))
		j--;
	new_cmd = (char *)malloc(sizeof(char) * (j - i + 2));
	if (!new_cmd)
		return (NULL);
	z = 0;
	while (i <= j)
		new_cmd[z++] = cmd[i++];
	new_cmd[z] = '\0';
	return (new_cmd);
}

long long	count_cmds(char *cmd)
{
	int			parenth;
	int			simple_qu;
	int			double_qu;
	size_t		i;
	long long	nb_cmds;

	parenth = 0;
	simple_qu = 0;
	double_qu = 0;
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' && simple_qu == 0)
		{
			double_qu ^= 1;
			i++;
		}
		else if (cmd[i] == 39 && double_qu == 0)
		{
			simple_qu ^= 1;
			i++;
		}
		else if (cmd[i] == '(' && simple_qu == 0 && double_qu == 0)
		{
			parenth++;
			i++;
		}
		else if (cmd[i] == ')' && simple_qu == 0 && double_qu == 0)
		{
			if (!parenth)
				return (0);
			parenth--;
			i++;	
		}
		else if (((cmd[i] == '&' && cmd[i + 1] == '&')
				|| (cmd[i] == '|' && cmd[i + 1] == '|'))
				&& !parenth && !double_qu && !simple_qu)
		{
			nb_cmds++;
			i += 2;
		}
		else
			i++;
	}
	if (parenth || double_qu || simple_qu)
		return (0);
	nb_cmds++;
	return (nb_cmds);
}

t_cmd	*add_cmd(char *cmd, size_t *i)
{
	size_t			j;
	int				parenth;
	int				simple_qu;
	int				double_qu;
	t_cmd			*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
	j = 0;
	parenth = 0;
	simple_qu = 0;
	double_qu = 0;
	while (cmd[*i + j])
	{
		if (cmd[*i + j] == '"' && simple_qu == 0)
		{
			double_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == 39 && double_qu == 0)
		{
			simple_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == '(' && simple_qu == 0 && double_qu == 0)
		{
			parenth++;
			j++;
		}
		else if (cmd[*i + j] == ')' && simple_qu == 0 && double_qu == 0)
		{
			parenth--;
			j++;	
		}
		else if (((cmd[*i + j] == '&' && cmd[*i + j + 1] == '&')
				|| (cmd[*i + j] == '|' && cmd[*i + j + 1] == '|'))
				&& !parenth && !double_qu && !simple_qu)
		{
			if (cmd[*i + j] == '|')
				new_cmd->next_delim = 3;
			else if (cmd[*i + j] == '&')
				new_cmd->next_delim = 2;
			break ;
		}
		else
			j++;
	}
	if (!cmd[*i + j])
		new_cmd->next_delim = 0;
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	if (new_cmd->next_delim == 2 || new_cmd->next_delim == 3)
		*i += j + 2;
	return (new_cmd);
}

int	is_wrapped_p(char *cmd)
{
	size_t	i;
	int		double_qu;
	int		simple_qu;
	int		parenth;

	double_qu = 0;
	simple_qu = 0;
	if (cmd[0] != '(')
		return (0);
	i = 0;
	parenth = 1;
	i++;
	while (cmd[i])
	{
		if (cmd[i] == '(' && !double_qu && !simple_qu)
		{
			parenth++;
			i++;
		}
		if (cmd[i] == ')' && !double_qu && !simple_qu)
		{
			if (parenth == 1)
				break;
			else
				parenth--;
			i++;
		}
		else if (cmd[i] == '"' && !simple_qu)
		{
			double_qu ^= 1;
			i++;
		}
		else if (cmd[i] == 39 && !double_qu)
		{
			simple_qu ^= 1;
			i++;
		}
		else
			i++;
	}
	if (cmd[i] != ')')
		return (2);
	i++;
	if (cmd[i] == '\0')
		return (1);
	while (cmd[i] && ((cmd[i] >= '\t' && cmd[i] <= '\r') || cmd[i] == ' '))
		i++;
	if ((cmd[i] == '&' && cmd[i + 1] == '&') || cmd[i] == '|')
		return (0);
	else if (cmd[i] == '>')
	{
		i++;
		if (cmd[i] && cmd[i] == '>')
			i++;
		while (cmd[i] && ((cmd[i] >= '\t' && cmd[i] <= '\r') || cmd[i] == ' '))
			i++;
		if (cmd[i] == '\0')
			return (2);
		while (cmd[i] && cmd[i] > 32 && cmd[i] < 127)
			i++;
		if (cmd[i] == '\0')
			return (1);
		while (cmd[i] && ((cmd[i] >= '\t' && cmd[i] <= '\r') || cmd[i] == ' '))
			i++;
		if (cmd[i] != '|' && cmd[i] != '&')
		   return (2);
		else
			return (0);
	}
	return (2);
}

int	parse_logical(t_cmd *cmd)
{
	size_t		index;
	long long	i;
	long long	j;

	if (!cmd || !cmd->cmd)
		return (1);
	if (cmd->cmd[0] == '&' || cmd->cmd[0] == '|')
		return (write(2, "Parsing error\n", 14), 1);
	i = count_cmds(cmd->cmd);
	if (!i)
		return (write(2, "Parsing error\n", 14), 1);
	else if (i == 1)
		return (0);
	cmd->sub_cmd = (t_cmd **)malloc(sizeof(t_cmd *) * (i + 1));
	if (!cmd->sub_cmd)
		return (1);
	j = 0;
	index = 0;
	while (j < i)
	{
		cmd->sub_cmd[j] = add_cmd(cmd->cmd, &index);
		if (!cmd->sub_cmd[j])
			return (1);
		if (j != 0)
			cmd->sub_cmd[j]->prev_delim = cmd->sub_cmd[j - 1]->next_delim;
		if (parse_cmd(cmd->sub_cmd[j]))
			return (1);
		j++;
	}
	cmd->sub_cmd[0]->prev_delim = 0;
	cmd->sub_cmd[i - 1]->next_delim = 0;
	cmd->sub_cmd[i] = NULL;
	return (i);
}

t_cmd	*add_pipe(char *cmd, size_t *i)
{
	size_t			j;
	int				parenth;
	int				simple_qu;
	int				double_qu;
	t_cmd			*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
	j = 0;
	parenth = 0;
	simple_qu = 0;
	double_qu = 0;
	while (cmd[*i + j])
	{
		if (cmd[*i + j] == '"' && simple_qu == 0)
		{
			double_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == 39 && double_qu == 0)
		{
			simple_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == '(' && simple_qu == 0 && double_qu == 0)
		{
			parenth++;
			j++;
		}
		else if (cmd[*i + j] == ')' && simple_qu == 0 && double_qu == 0)
		{
			parenth--;
			j++;	
		}
		else if (cmd[*i + j] == '|' && !parenth && !double_qu && !simple_qu)
			break ;
		else
			j++;
	}
	cmd[*i + j] = '\0';
	new_cmd->cmd = ft_trim(&cmd[*i]);
	*i += j + 1;
	return (new_cmd);
}

long long	count_pipes(char *cmd)
{
	int			parenth;
	int			simple_qu;
	int			double_qu;
	size_t		i;
	long long	nb_cmds;

	parenth = 0;
	simple_qu = 0;
	double_qu = 0;
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' && simple_qu == 0)
		{
			double_qu ^= 1;
			i++;
		}
		else if (cmd[i] == 39 && double_qu == 0)
		{
			simple_qu ^= 1;
			i++;
		}
		else if (cmd[i] == '(' && simple_qu == 0 && double_qu == 0)
		{
			parenth++;
			i++;
		}
		else if (cmd[i] == ')' && simple_qu == 0 && double_qu == 0)
		{
			if (!parenth)
				return (0);
			parenth--;
			i++;	
		}
		else if (cmd[i] == '|' && !parenth && !double_qu && !simple_qu)
		{
			nb_cmds++;
			i ++;
		}
		else
			i++;
	}
	if (parenth || double_qu || simple_qu || cmd[i - 1] == '|')
		return (0);
	nb_cmds++;
	return (nb_cmds);

}

int	parse_pipe(t_cmd *cmd)
{
	size_t		index;
	long long	i;
	long long	j;

	if (!cmd || !cmd->cmd)
		return (1);
	if (cmd->cmd[0] == '&' || cmd->cmd[0] == '|')
		return (write(2, "Parsing error\n", 14), 1);
	i = count_pipes(cmd->cmd);
	if (!i)
		return (write(2, "Parsing error\n", 14), 1);
	else if (i == 1)
		return (0);
	cmd->pipe = (t_cmd **)malloc(sizeof(t_cmd *) * (i + 1));
	if (!cmd->pipe)
		return (1);
	j = 0;
	index = 0;
	while (j < i)
	{
		cmd->pipe[j] = add_pipe(cmd->cmd, &index);
		if (!cmd->pipe[j])
			return (1);
		cmd->pipe[j]->prev_delim = 1;
		cmd->pipe[j]->next_delim = 1;
		if (parse_cmd(cmd->pipe[j]))
			return (1);
		j++;
	}
	cmd->pipe[0]->prev_delim = 0;
	cmd->pipe[i - 1]->next_delim = 0;
	cmd->pipe[i] = NULL;
	return (i);
}

void	strip_parenth(char *cmd)
{
	long long	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '(')
			break;
		i++;
	}
	if (cmd[i] == '(')
		cmd[i] = ' ';
	while (cmd[i])
		i++;
	while (i >= 0 && cmd[i] != ')')
		i--;
	if (cmd[i] == ')')
		cmd[i] = ' ';
}

int	parse_simple_cmd(t_cmd *cmd)
{
	cmd->cmd_name = cmd->cmd;
	return (0);
}

int	parse_cmd(t_cmd *cmd)
{
	char	*trimmed_cmd;
	int		ret;

	trimmed_cmd = ft_trim(cmd->cmd);
	if (!trimmed_cmd)
		return (write(2, "Parsing error\n", 14), 1);
	free(cmd->cmd);
	cmd->cmd = trimmed_cmd;
	cmd->fork = 0;
	cmd->pipe = NULL;
	cmd->sub_cmd = NULL;
	ret = is_wrapped_p(cmd->cmd);
	if (ret == 2)
		return (1);
	else if (ret)
	{
		cmd->fork = 1;
		strip_parenth(cmd->cmd);
		trimmed_cmd = ft_trim(cmd->cmd);
		if (!trimmed_cmd)
			return (write(2, "Parsing error\n", 14), 1);
		free(cmd->cmd);
		cmd->cmd = trimmed_cmd;
	}
	ret = parse_logical(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = parse_pipe(cmd);
	if (ret == 1)
		return (1);
	else if (ret)
		return (0);
	ret = parse_simple_cmd(cmd);
	if (ret)
		return (1);
	return (0);
}

void	handle_signal(int signal)
{
	if (signal == SIGKILL)
		g_signal = 1;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int main(int argc, char **argv, char **envp)
{
	char				*cmd;
	t_info				info;

	if (argc != 1)
		return (1);
	if (create_info(&info, envp, argv[0]))
		return (1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	while (1)
	{
		cmd = readline("Minishell: ");
		if (cmd == NULL)
		{
			free_info(&info);
			return (0);
		}
		if (cmd[0] != '\0')
		{
			add_history(cmd);
			info.cmd.cmd = ft_trim(cmd);
			free(cmd);
			if (!info.cmd.cmd)
				free(info.cmd.cmd);
			else
			{
				info.cmd.prev_delim = 0;
				info.cmd.next_delim = 0;
				if (!parse_cmd(&info.cmd))
				{
					execute_command(&info.cmd);
					printf("\n");
				}
				else
					write(2, "Error\n", 6);
			}
		}
		else
			free(cmd);
	}
	return (0);
}

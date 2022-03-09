//gcc main.c -lreadline

#include "minishell.h"

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

void	free_info(t_info *info)
{
	free_env(info);
	free_var(info);
	free_running_processes(info);
	free_cmd(info);
}

int	create_info(t_info *info, char **envp, char *name)
{
	size_t	i;

	info->prog_name = name;
	info->local_var = NULL;
	info->running_processes = NULL;
	info->last_ret = 0;
	info->env = NULL;
	info->cmd = NULL;
	i = 0;
	while (envp[i])
		i++;
	while (i--)
	{
		if (add_to_env(info, envp[i]))
			return (free_env(info), 1);
	}
	return (0);
}

char *print_delim(int delim)
{
	 return (delim == 0 ? ("None") : (delim == 1 ? ("|") : (delim == 2 ? ("&&") : ("||"))));
}

void	execute_command(t_info *info)
{
	size_t	i;

	i = 0;
	while (info->cmd[i])
	{
		printf("Command %lu: %s, previous delimiter: %s, next delimiter %s\n", i, info->cmd[i]->cmd, print_delim(info->cmd[i]->prev_delim) , print_delim(info->cmd[i]->next_delim));
		i++;
	}
/*	printf("Trimmed command: %s\n", info->cmd);
		return ;
	// exit if necessary
	else if (ft_strncmp(info->cmd->cmd, "exit", 4)
			&& (info->cmd->cmd[4] == '\0' || ft_whitespace(info->cmd->cmd[4])))
	{
		exit_code = get_args(&info->cmd->cmd[4]);
	}*/
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
		if (cmd[i] == '"')
		{
			double_qu ^= 1;
			i++;
		}
		else if (cmd[i] == 39)
		{
			simple_qu ^= 1;
			i++;
		}
		else if (cmd[i] == '(' || (cmd[i] == '$' && cmd[i + 1] == '('))
		{
			parenth++;
			if (cmd[i] == '$')
				i += 2;
			else
				i++;
		}
		else if (cmd[i] == ')')
		{
			if (!parenth)
				return (-1);
			parenth--;
			i++;	
		}
		else if ((cmd[i] == '&' || cmd[i] == '|') && !parenth && !double_qu && !simple_qu)
		{
			nb_cmds++;
			if ((cmd[i] == '|' && cmd[i + 1] == '|')
				|| (cmd[i] == '&' && cmd[i + 1] == '&'))
				i += 2;
			else if (cmd[i] == '&' && cmd[i + 1] != '&')
				return (-1);
			else
				i++;
		}
		else
			i++;
	}
	if (parenth || double_qu || simple_qu)
		return (-1);
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
		if (cmd[*i + j] == '"')
		{
			double_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == 39)
		{
			simple_qu ^= 1;
			j++;
		}
		else if (cmd[*i + j] == '(' || (cmd[*i + j] == '$' && cmd[*i + j + 1] == '('))
		{
			parenth++;
			if (cmd[*i + j] == '$')
				j += 2;
			else
				j++;
		}
		else if (cmd[*i + j] == ')')
		{
			parenth--;
			j++;	
		}
		else if ((cmd[*i + j] == '&' || cmd[*i + j] == '|') && !parenth && !double_qu && !simple_qu)
		{
			if (cmd[*i + j] == '|' && cmd[*i + j + 1] == '|')
				new_cmd->next_delim = 3;
			else if (cmd[*i + j] == '&' && cmd[*i + j + 1] == '&')
				new_cmd->next_delim = 2;
			else
				new_cmd->next_delim = 1;
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
	else if(new_cmd->next_delim == 1)
		*i += j + 1;
	else
		*i += j;
	return (new_cmd);
}

int	ft_parse_cmd(t_info *info, char *cmd, int first_delim, int last_delim)
{
	size_t		index;
	long long	i;
	long long	j;

	if (cmd == NULL)
		return (1);
	if (cmd[0] == '&' || cmd[0] == '|')
		return (free(cmd), write(2, "Parsing error\n", 14), 1);
	i = count_cmds(cmd);
	if (i <= 0)
		return (free(cmd), write(2, "Parsing error\n", 14), 1);
	info->cmd = (t_cmd **)malloc(sizeof(t_cmd *) * (i + 1));
	if (!info->cmd)
		return (free(cmd), 1);
	j = 0;
	index = 0;
	while (j < i)
	{
		info->cmd[j] = add_cmd(cmd, &index);
		if (!info->cmd[j])
			return (free_cmd(info), free(cmd), 1);
		if (j != 0)
			info->cmd[j]->prev_delim = info->cmd[j - 1]->next_delim;
		j++;
	}
	info->cmd[0]->prev_delim = first_delim;
	info->cmd[i - 1]->next_delim = last_delim;
	free(cmd);
	info->cmd[i] = NULL;
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	char	*cmd;
	char	*trimmed_cmd;
	t_info	info;

	if (!envp || !envp[0])
		return (1);
	if (create_info(&info, envp, argv[0]))
		return (1);
	while (1)
	{
		cmd = readline("Minishell: ");
		if (cmd != NULL && cmd[0] != '\0')
		{
			add_history(cmd);
			trimmed_cmd = ft_trim(cmd);
			free(cmd);
			if (!ft_parse_cmd(&info, trimmed_cmd, 0, 0))
				execute_command(&info);
		}
		else
			free(cmd);
	}
	return (0);
}

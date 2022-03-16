//gcc main.c -lreadline

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

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
	int			is_tok;
	t_tokens	toks;
	size_t		i;
	long long	nb_cmds;

	init_tokens(&toks);
	nb_cmds = 0;
	i = 0;
	while (cmd[i])
	{
		is_tok = save_token(cmd[i], &toks);
		if (is_tok == 2)
			return (0);
		else if (!is_tok && ((cmd[i] == '&' && cmd[i + 1] == '&')
				|| (cmd[i] == '|' && cmd[i + 1] == '|'))
				&& !toks.par && !toks.dbl_qu && !toks.spl_qu)
		{
			nb_cmds++;
			i++;
		}
			i++;
	}
	if (toks.par || toks.dbl_qu || toks.spl_qu || cmd[i - 1] == '|' 
		|| cmd[i - 1] == '&')
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

void	skip_withespaces(char *cmd, int *i)
{
	while (cmd[*i] && ((cmd[*i] >= '\t' && cmd[*i] <= '\r') || cmd[*i] == ' '))
		*i += 1;
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

int	init_cmd(t_cmd *cmd)
{
	char	*trimmed_cmd;

	trimmed_cmd = ft_trim(cmd->cmd);
	if (!trimmed_cmd)
		return (write(2, "Parsing error\n", 14), 1);
	free(cmd->cmd);
	cmd->cmd = trimmed_cmd;
	cmd->fork = 0;
	cmd->pipe = NULL;
	cmd->sub_cmd = NULL;

	return (0);
}

int	parse_cmd(t_cmd *cmd)
{
	int		ret;


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

	if (argc != 1 || create_info(&info, envp, argv[0]))
		return (1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	while (1)
	{
		cmd = readline("Minishell: ");
		if (cmd == NULL)
			return (free_info(&info), info.last_ret);
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

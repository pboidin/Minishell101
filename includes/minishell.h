/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:51:46 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/16 20:23:30 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_redirect
{
	int					type;
	char				*str;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	int				prev_delim;
	struct s_cmd	*fork;
	t_redirect		*in;
	t_redirect		*out;
	char			*cmd;
	char			*cmd_name;
	char			**cmd_args;
	struct s_cmd	**pipe;
	struct s_cmd	**sub_cmd;
	int				next_delim;
} t_cmd;

typedef struct s_tokens
{
	int	par;
	int	dbl_qu;
	int	spl_qu;
}	t_tokens;

typedef struct s_pid
{
	pid_t			pid;
	struct s_pid	*next;
}	t_pid;

typedef struct s_var
{
	char			*name;
	char			*value;
	struct s_var	*next;
}	t_var;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_info
{
	char	*prog_name;
	t_env	*env;
	t_var	*local_var;
	t_pid	*running_processes;
	int		last_ret;
	t_cmd	cmd;
}	t_info;

void	free_env(t_info *info);
int		create_info(t_info *info, char **envp, char *name);
int		parse_cmd(t_cmd *cmd);
void	init_tokens(t_tokens *tokens);
int		save_token(char c, t_tokens *toks);
void	free_info(t_info *info);
char	*ft_trim(char *cmd);
int		parse_pipe(t_cmd *cmd);
int		has_tokens(t_tokens toks);
int		is_delim(char *str, int delim);
void	save_delim(t_cmd *new_cmd, char c);
int		parse_logical(t_cmd *cmd);
void	skip_whitespaces(char *cmd, int *i);
int		fork_cmd(t_cmd *cmd);
int		parse_cmd(t_cmd *cmd);
int		parse_simple_cmd(t_cmd *cmd);
//int	ft_strncmp(const char *s1, const char *s2, size_t n);

//size_t	ft_strlen(const char *s);

//void	ft_putchar(char c);
//void	ft_putendl_fd(char *str, int fd);
//void	ft_putstr(char *str);

#endif

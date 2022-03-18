/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:51:46 by bdetune           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/03/17 14:18:27 by bdetune          ###   ########.fr       */
=======
/*   Updated: 2022/03/11 19:38:41 by piboidin         ###   ########.fr       */
>>>>>>> 98fcf121b1ab300f08d5762ed32ea57740b52997
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

<<<<<<< HEAD
typedef struct s_redirect
{
	int					type;
	char				*str;
	struct s_redirect	*next;
}	t_redirect;
=======
# ifndef TRUE
#  define TRUE 1
# endif
# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;
>>>>>>> 98fcf121b1ab300f08d5762ed32ea57740b52997

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

typedef int t_bool;

typedef struct s_info
{
	int		status; // last exit status  
	char	*prog_name;
	t_env	**env; //environment vars
	t_var	*local_var;
	t_pid	*running_processes;
	int		last_ret;
<<<<<<< HEAD
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
int		parse_args(t_cmd *cmd);

//int	ft_strncmp(const char *s1, const char *s2, size_t n);
=======
	t_cmd	**cmd;
	t_bool	is_child; // if a process is a child
}	t_info;

extern t_info	g_info;
>>>>>>> 98fcf121b1ab300f08d5762ed32ea57740b52997

int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_islower(int c);
int		ft_isupper(int c);
int		ft_lenvar(const char *s);
int		ft_lstsize(t_env *lst);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

char	*ft_genv(const char *path);
char	*ft_strdup(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *Str, size_t n, t_bool free_str);
char	*ft_substr(char const *s, unsigned int start, size_t len);

size_t	ft_strlen(const char *s);

t_env	*ft_lstnew(void *data);

//void	ft_putchar(char c);
//void	ft_putendl_fd(char *str, int fd);
//void	ft_putstr(char *str);

void	ft_export_var(char **new_env, char *tmp, char *env, char *env2);
void	ft_lstclear(t_env **lst, void (*del)(void *));
void	ft_lstadd_back(t_env **lst, t_env *new);
void	*ft_memset(void *target, int char_to_set, size_t n);

#endif

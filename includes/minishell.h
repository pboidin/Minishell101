/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:51:46 by bdetune           #+#    #+#             */
/*   Updated: 2022/04/10 17:01:13 by piboidin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# ifndef TRUE
#  define TRUE 1
# endif
# ifndef FALSE
#  define FALSE 0
# endif
# ifndef OR
#  define OR 3
# endif
# ifndef AND
#  define AND 2
# endif
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_redirect
{
	int					type;
	char				*str;
	char				*path;
	int					fd;
	int					var_expansion;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	int				prev_delim;
	struct s_cmd	*fork;
	t_redirect		*redirections;
	t_redirect		*in;
	t_redirect		*out;
	char			*cmd;
	char			*cmd_name;
	char			**cmd_args;
	struct s_cmd	**pipe;
	struct s_cmd	**sub_cmd;
	int				next_delim;
}	t_cmd;

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

typedef struct s_block
{
	char	*str;
	int		spl_qu;
	int		dbl_qu;
	int		var;
}	t_block;

extern t_env	g_env;

typedef	int	t_bool;

typedef struct s_info
{
	int		status;
	char	*prog_name;
	t_env	*env;
	t_var	*local_var;
	t_pid	*running_processes;
	t_cmd	cmd;
	t_bool	is_child;
}	t_info;

extern t_info	g_info;

void	free_env(t_info *info);
int		create_info(t_info *info, char **envp, char *name);
int		parse_cmd(t_cmd *cmd);
void	init_tokens(t_tokens *tokens);
void	init_cmd(t_cmd *cmd);
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
void	free_cmd(t_cmd *cmd);
char	**join_env(t_info *info);
int		save_heredoc(t_redirect *new_redirect);
char	*ft_del_spaces(char *str);
char	*ft_strdup(const char *s);
int		ft_strcmp(char *s1, char *s2);
char	*ft_itoa(int n);
char	**ft_split_charset(char const *s, char *set);

extern t_info	g_info;

/* BUILT-IN */

int		ft_ch_dir(char **dir, t_info *info);
int		ft_echo(char **tab);
int		ft_env(char **tab, t_info *info);
int		ft_exit(char **tab, t_info *info);
int		ft_export(char **new_env, t_info *info);
int		ft_working_dir(char **n, t_info *info);
int		ft_unset(char **unset, t_info *info);
int		ft_blt(t_cmd *cmd);
void	ft_blti(t_info *info, t_cmd *cmd);

void	ft_execute(t_info *info, char **cmd_args);
char	**ft_split(char const *s, char c);
int		ft_env_loc(t_env *head);
int		ft_go_to_home(t_info *info);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_islower(int c);
int		ft_isupper(int c);
int		ft_lenvar(const char *s);
int		ft_lstsize(t_env *lst);
int		ft_print_err_cd(const char *dir);
int		ft_ret_home(t_info *info);
int		ft_set_old(t_info *info, char *pwd, char *val);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_unset_handle(char *new_env);
int		ft_atoi(const char *str);
void	general_controller(t_info *info, t_cmd *cmd);
int		add_pid(t_info *info, int pid);
void	free_pid(t_info *info);
int		ft_abs(int nb);

char	*ft_genv(const char *path, t_info *info);
char	*ft_strdup(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *Str, size_t n, t_bool free_str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_calloc(size_t quantity, size_t type_size);
void	ft_bzero(void *s, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);

size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);

t_env	*ft_lstnew(void *data);

void	ft_env_set(t_info *info);
void	ft_export_var(char **new_env, char *env, char *env2, t_info *info);
void	ft_lstclear(t_env **lst, void (*del)(void *));
void	ft_lstadd_back(t_env **lst, t_env *new);
void	ft_lstdelone(t_env *lst, void (*del)(void*));
void	ft_upd_env(t_info *info);
void	*ft_memcpy(void *dst, const void *src, size_t memSize);
void	*ft_memset(void *target, int char_to_set, size_t n);
void	*ft_realloc(void *ptr, size_t memSize);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:51:46 by bdetune           #+#    #+#             */
/*   Updated: 2022/05/13 19:51:43 by bdetune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
# include <stdio.h>
# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <stddef.h>
# include <sys/uio.h>
# include <stdbool.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 500
# endif
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
# ifndef PIPE
#  define PIPE 1
# endif
# ifndef PARENTH
#  define PARENTH 4
# endif
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif
# ifndef TMP_PATH
#  define TMP_PATH "/tmp/minishell-"
# endif

extern volatile sig_atomic_t	g_signal;
typedef int						t_bool;

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
	char			**joined_env;
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

typedef struct s_wild {
	char			*path;
	struct s_wild	*next;
}	t_wild;

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

char	*add_t_block_str(char *str, size_t *index);
char	*create_tmp(void);
char	*find_var(char *var, t_info *info);
char	*ft_del_spaces(char *str);
char	*ft_delete_wild(t_block *src, int *depth);
char	*ft_genv(const char *path, t_env *info);
char	*ft_getvalue(char *str, t_env *env);
char	*ft_itoa(int n);
char	*ft_strcat(char *dest, char *src);
char	*ft_strcat_mal(char *str1, char *str2);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *Str, size_t n, t_bool free_str);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_trim(char *cmd);
char	*get_full_path(char *path, char *new_file, size_t path_len);
char	*get_var_name(char *str, t_info *info);
char	*get_var_val(char *str, t_info *info, char *var[2]);
char	*internal_get_line(char *remainer, int *nl_pos);
char	*internal_get_str(int fd, char *remainer, int start_index, int *nl_pos);
char	*internal_getremainer(char *remainer, int nl_pos);
char	*internal_join(char *dst, char *src);
char	*t_block_to_str(t_block *block);

char	**create_char_tab(size_t size);
char	**ft_split(char const *s, char c);
char	**ft_split_charset(char const *s, char *set);
char	**join_env(t_info *info);
char	**replace_var(t_block *words, size_t i, t_info *info);
char	**t_block_tab_to_char_tab(t_block **tab);

DIR		*open_dir(const char *path, size_t *path_len);

int		add_pid(t_info *info, int pid);
int		add_redirect(char *str, t_cmd *cmd, int redirect);
int		array_len(char **dir);
int		clean_previous_args(t_cmd *cmd, int *i);
int		create_info(t_info *info, char **envp, char *name);
int		end_wilderness(t_block **mask);
int		expand_var(t_info *info, t_block ***words_tab, size_t i[2], int expand);
int		export_expansion(t_info *info, t_cmd *cmd);
int		final_match(char *name, char *full_path, t_block **mask);
int		fork_cmd(t_cmd *cmd);
int		ft_abs(int nb);
int		ft_atoi(const char *str);
int		ft_blt(t_cmd *cmd);
int		ft_cd(char **dir, t_info *info);
int		ft_ch_dir(char **dir, t_info *info);
int		ft_compare(t_block *mask, char *path);
int		ft_echo(char **tab, t_cmd *cmd);
int		ft_env(char **tab, t_info *info, t_cmd *cmd);
int		ft_env_loc(t_env *head);
int		ft_error_export(char *new_env);
int		ft_exit(char **tab, t_info *info);
int		ft_export(char **new_env, t_info *info);
int		ft_go_to_home(t_info *info);
int		ft_has_env(char *str, t_env *env);
int		ft_has_wildcards(t_block *block);
int		ft_home_err_prt(void);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_islower(int c);
int		ft_isupper(int c);
int		ft_lenvar(const char *s);
int		ft_lstlen(t_wild *list);
int		ft_lstsize(t_env *lst);
int		ft_print_err_cd(const char *dir);
int		ft_ret_home(t_info *info);
int		ft_set_old(t_env *env, char *pwd);
int		ft_strchr_wild(char *str, char c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_unset(char **unset, t_info *info);
int		ft_unset_handle(char *new_env);
int		ft_working_dir(char **n, t_info *info, t_cmd *cmd);
int		get_final_cmd(t_cmd *cmd, t_info *info);
int		get_next_line(int fd, char **line);
int		get_redirect_type(char *str);
int		handle_assignation(t_cmd *cmd, t_info *info);
int		handle_redirections(t_cmd *cmd, t_info *info);
int		has_tokens(t_tokens toks);
int		inline_expansion(t_block *tab, size_t i, t_info *info);
int		internal_hasnl(char *str, int *start_index, int *nl_pos);
int		is_assignation(char *str);
int		is_delim(char *str, int delim);
int		is_empty_var(t_block *word);
int		is_valid_arg(char *str);
int		is_valid_assignation(char *str);
int		is_whitespace(char c);
int		parse_args(t_cmd *cmd, char *str);
int		parse_cmd(t_cmd *cmd);
int		parse_logical(t_cmd *cmd);
int		parse_pipe(t_cmd *cmd);
int		parse_simple_cmd(t_cmd *cmd);
int		pipe_parent(t_cmd *cmd, size_t i, int fd[3]);
int		remove_qu(t_block *tab, size_t i);	
int		save_heredoc(t_redirect *new_redirect);
int		save_token(char c, t_tokens *toks);

size_t	char_tab_size(char **tab);
size_t	count_words_var_expansion(char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
size_t	split_tab_var(t_block ***words_tab, size_t j, size_t i, char **var);
size_t	t_block_tab_size(t_block **tab);

t_block	**add_args_word(char *str, t_info *info, int expand);
t_block	**add_block_to_tab(t_block **old_tab, t_block **to_add);
t_block	*add_str_t_block(t_block *spl, char *str, int i, char wild);
t_block	**add_to_mask(int s[2], int e[2], char wild, t_block *block);
t_block	**build_mask(t_block *block);
t_block	**create_t_tab(char *str);
t_block	**replace_wild_cards(t_block **words_tab);
t_block	*split_on_wild(t_block **new_block, t_block *block, int s[2]);
t_block	**wild_one(t_block *block);

t_env	*find_env_var(t_info *info, char *var_name);
t_env	*ft_lstnew(void *data);
t_env	*ft_lstnew_export(char *tmp[2]);

t_var	*find_lcl_var(t_info *info, char *var_name);

t_wild	*add_to_list(t_wild *list, char *path, t_block **mask);
t_wild	*ft_lstlast_wild(t_wild *list);
t_wild	*ft_lstnew_wild(void *content);
t_wild	*print_dirs(char *path, t_block **mask);

void	add_lcl_var(t_info *info, char *var[2]);
void	ambiguous_redirect(char *str);
void	child_signal(int signal);
void	delete_empty(t_block *block);
void	execution_error(t_info *info, t_cmd *cmd, int exit_code, int absolute);
void	fork_child(t_info *info, t_cmd *cmd);
void	free_char_tab(char **tab);
void	free_cmd(t_cmd *cmd);
void	free_env(t_info *info);
void	free_info(t_info *info);
void	free_pid(t_info *info);
void	free_running_processes(t_info *info);
void	free_t_block(t_block *block);
void	free_t_block_tab(t_block **block_tab);
void	free_t_wild(t_wild *list);
void	free_var(t_info *info);
void	ft_add_env(char *const *new_env, int i, t_info *info, char *tmp[2]);
void	ft_blti(t_info *info, t_cmd *cmd);
void	ft_bzero(void *s, size_t n);
void	ft_env_set(t_env *env);
void	ft_error_print(int err, char *str);
void	ft_execute(t_info *info, t_cmd *cmd);
void	ft_export_var(char **new_env, t_info *info);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	ft_lstclear(t_env **lst, void (*del)(void *));
void	ft_lstdelone(t_env *lst, void (*del)(void*));
void	ft_modify_env(char *name, char *value, t_env *env);
void	ft_putendl_fd(char *str, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_upd_env(t_env *env);
void	ft_wild_two(t_block *src, char ***tab, t_wild *list);
void	general_controller(t_info *info, t_cmd *cmd);
void	get_exit_status(t_info *info);
void	handle_signal(int signal);
void	heredoc_signal(int signal);
void	init_cmd(t_cmd *cmd);
void	init_tokens(t_tokens *tokens);
void	move_t_block_tab_upward(t_block **tab, size_t i, int mv);
void	move_upward(t_cmd *cmd, int i, int mv);
void	move_upward_t_block_str(t_block *block, int i);
void	open_error(t_redirect *current);
void	parsing_error(int delim, char *str, t_tokens *toks);
void	pipe_child(t_info *info, t_cmd *cmd, size_t i, int fd[3]);
void	save_delim(t_cmd *new_cmd, char *c, int prev_delim, int next_delim);
void	simple_cmd_child(t_info *info, t_cmd *cmd);
void	skip_closing_parenth(char *str, int *i);
void	skip_englobing_char(char *str, size_t *i, char delim);
void	skip_to_end_var(char *str, size_t *i);
void	skip_whitespaces(char *cmd, int *i);
void	sys_call_error(t_info *info);
void	throw_assignation_error(char *str);
void	update_env_var(char *var[2], t_env *target);
void	update_lcl_var(char *var[2], t_var *target);
void	ft_update_var(char *var, char *name_var, t_info *info);

void	*ft_calloc(size_t quantity, size_t type_size);
void	*ft_memcpy(void *dst, const void *src, size_t memSize);
void	*ft_memset(void *target, int char_to_set, size_t n);
void	*ft_realloc(void *ptr, size_t memSize);

#endif

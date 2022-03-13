/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:51:46 by bdetune           #+#    #+#             */
/*   Updated: 2022/03/11 19:38:41 by piboidin         ###   ########.fr       */
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

# ifndef TRUE
#  define TRUE 1
# endif
# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_cmd
{
	int		prev_delim;
	char	*cmd;
	int		next_delim;
} t_cmd;

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
	t_cmd	**cmd;
	t_bool	is_child; // if a process is a child
}	t_info;

extern t_info	g_info;

int	ft_strncmp(const char *s1, const char *s2, size_t n);

char	*ft_genv(const char *path);

size_t	ft_strlen(const char *s);

//void	ft_putchar(char c);
//void	ft_putendl_fd(char *str, int fd);
//void	ft_putstr(char *str);

void	*ft_memset(void *target, int char_to_set, size_t n);

#endif

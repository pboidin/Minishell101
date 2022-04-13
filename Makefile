# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 13:02:16 by bdetune           #+#    #+#              #
#    Updated: 2022/04/13 11:54:50 by bdetune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Names
NAME		= minishell

SRCS = main.c \
	   srcs/init.c \
	   srcs/utils/free.c \
	   srcs/utils/free2.c \
	   srcs/parsing/tokens.c \
	   srcs/parsing/parse_pipes.c \
	   srcs/parsing/delim.c \
	   srcs/parsing/parse_logical.c \
	   srcs/utils/ft_utils_str.c \
	   srcs/utils/ft_utils_str2.c \
	   srcs/parsing/parse_fork.c \
	   srcs/parsing/parse_cmd.c \
	   srcs/parsing/parse_simple_cmd.c \
	   srcs/parsing/parse_cmd_args.c \
	   srcs/parsing/heredoc.c \
	   srcs/execution/controllers.c \
	   srcs/execution/pid.c \
	   srcs/execution/exit_status.c \
	   srcs/execution/var_expansion.c \
	   srcs/execution/var_expansion2.c \
	   srcs/execution/inline_expansion.c \
	   srcs/utils/env.c \
	   srcs/utils/create_tmp.c \
	   srcs/utils/move_upward.c \
	   srcs/utils/ft_utils_numbers.c \
	   srcs/execution/pipex_utils.c \
	   srcs/utils/ft_utils_function.c \
	   srcs/utils/ft_utils_function2.c \
	   srcs/utils/ft_utils_mem.c \
	   srcs/utils/ft_split.c \
	   srcs/utils/ft_itoa.c \
	   srcs/utils/ft_utils_alpha.c \
	   srcs/utils/ft_split_charset.c \
	   srcs/utils/ft_utils_lst.c \
	   srcs/utils/ft_utils_lst2.c \
	   srcs/utils/ft_atoi.c \
	   srcs/utils/t_block_utils.c \
	   srcs/utils/char_tab_utils.c \
	   srcs/utils/throw_error.c \
	   srcs/builtins/echo.c \
	   srcs/builtins/pwd.c \
	   srcs/builtins/cd.c \
	   srcs/builtins/cd_utils.c \
	   srcs/builtins/cd_error.c \
	   srcs/builtins/env.c \
	   srcs/builtins/unset.c \
	   srcs/builtins/unset_utils.c \
	   srcs/builtins/export.c \
	   srcs/builtins/export_utils.c \
	   srcs/builtins/export_utils_2.c \
	   srcs/builtins/exit.c \
	   srcs/builtins/init_built.c

OBJS := ${SRCS:.c=.o}

NAME = minishell
INCL:= includes/minishell.h
CC= gcc
CFLAGS= -Wall -Werror -Wextra

${NAME}:	${INCL} ${OBJS}
		${CC} $(CFLAGS) -o ${NAME} ${OBJS} -lreadline
	
all:		$(NAME)

$(OBJS):		$(INCL)

.SECONDARY:	$(OBJS)

clean:		
		rm -rf ${OBJS}

fclean:		clean
			rm -rf ${NAME}

re:		fclean all

.c.o:		
		$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} -Iincludes -lreadline


.PHONY:	all clean fclean re bonus

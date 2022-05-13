# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: piboidin <piboidin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 13:02:16 by bdetune           #+#    #+#              #
#    Updated: 2022/05/13 15:18:45 by bdetune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Names
NAME = minishell

SRCS = main.c \
		srcs/builtins/cd_error.c \
		srcs/builtins/cd_utils.c \
		srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/exit.c \
		srcs/builtins/export_utils_2.c \
		srcs/builtins/export_utils.c \
		srcs/builtins/export.c \
		srcs/builtins/init_built.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset_utils.c \
		srcs/builtins/unset.c \
		srcs/execution/assignation.c \
		srcs/execution/child_signals.c \
		srcs/execution/childs.c \
		srcs/execution/controllers.c \
		srcs/execution/exit_status.c \
		srcs/execution/export_expansion.c \
		srcs/execution/ft_execute.c \
		srcs/execution/get_final_cmd.c \
		srcs/execution/inline_expansion.c \
		srcs/execution/pid.c \
		srcs/execution/redirections.c \
		srcs/execution/split_tab_var.c \
		srcs/execution/var_expansion.c \
		srcs/execution/var_expansion2.c \
		srcs/execution/var_expansion3.c \
		srcs/parsing/assignation.c \
		srcs/parsing/delim.c \
		srcs/parsing/heredoc.c \
		srcs/parsing/parse_cmd_args.c \
		srcs/parsing/parse_cmd.c \
		srcs/parsing/parse_fork.c \
		srcs/parsing/parse_logical.c \
		srcs/parsing/parse_pipes.c \
		srcs/parsing/parse_simple_cmd.c \
		srcs/parsing/redirections.c \
		srcs/parsing/tokens.c \
		srcs/utils/args.c \
		srcs/utils/char_tab_utils.c \
		srcs/utils/create_tmp.c \
		srcs/utils/env.c \
		srcs/utils/free.c \
		srcs/utils/free2.c \
		srcs/utils/ft_atoi.c \
		srcs/utils/ft_itoa.c \
		srcs/utils/ft_split_charset.c \
		srcs/utils/ft_split.c \
		srcs/utils/ft_strstr.c \
		srcs/utils/ft_utils_alpha.c \
		srcs/utils/ft_utils_function.c \
		srcs/utils/ft_utils_function2.c \
		srcs/utils/ft_utils_lst.c \
		srcs/utils/ft_utils_lst2.c \
		srcs/utils/ft_utils_mem.c \
		srcs/utils/ft_utils_numbers.c \
		srcs/utils/ft_utils_str.c \
		srcs/utils/ft_utils_str2.c \
		srcs/utils/ft_utils_str3.c \
		srcs/utils/get_next_line_utils.c \
		srcs/utils/get_next_line.c \
		srcs/utils/init.c \
		srcs/utils/lcl_var.c \
		srcs/utils/move_upward.c \
		srcs/utils/t_block_utils.c \
		srcs/utils/throw_error.c \
		srcs/utils/wild.c \
		srcs/utils/wild1.c \
		srcs/utils/wild2.c \
		srcs/utils/wild3.c

OBJS := ${SRCS:.c=.o}

NAME = minishell
INCL:= includes/minishell.h
CC= gcc
CFLAGS= -Wall -Werror -Wextra -g3

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

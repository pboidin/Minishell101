# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 13:02:16 by bdetune           #+#    #+#              #
#    Updated: 2022/03/22 14:34:59 by bdetune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c \
	   srcs/init.c \
	   srcs/utils/free.c \
	   srcs/parsing/tokens.c \
	   srcs/parsing/parse_pipes.c \
	   srcs/parsing/delim.c \
	   srcs/parsing/parse_logical.c \
	   srcs/utils/ft_utils_str.c \
	   srcs/parsing/parse_fork.c \
	   srcs/parsing/parse_cmd.c \
	   srcs/parsing/parse_simple_cmd.c \
	   srcs/parsing/parse_cmd_args.c \
	   srcs/execution/controllers.c \
	   srcs/execution/pid.c \
	   srcs/utils/env.c \
	   srcs/execution/pipex_utils.c \
	   ft_utils_function.c \
	   ft_utils_function2.c \
	   ft_utils_mem.c \
	   srcs/utils/ft_split.c

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

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 13:02:16 by bdetune           #+#    #+#              #
#    Updated: 2022/03/22 11:39:13 by bdetune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c \
	   srcs/init.c \
	   srcs/free.c \
	   srcs/tokens.c \
	   srcs/parse_pipes.c \
	   srcs/delim.c \
	   srcs/parse_logical.c \
	   srcs/ft_utils_str.c \
	   srcs/parse_fork.c \
	   srcs/parse_cmd.c \
	   srcs/parse_simple_cmd.c \
	   srcs/parse_cmd_args.c \
	   srcs/controllers.c \
	   srcs/pid.c \
	   srcs/env.c \
	   srcs/pipex_utils.c \
	   ft_utils_function.c \
	   ft_utils_function2.c \
	   ft_utils_mem.c \
	   srcs/ft_split.c

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

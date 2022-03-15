# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdetune <bdetune@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 13:02:16 by bdetune           #+#    #+#              #
#    Updated: 2022/03/15 11:52:48 by bdetune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c

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

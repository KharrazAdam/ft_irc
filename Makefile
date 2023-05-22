# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/22 16:06:48 by akharraz          #+#    #+#              #
#    Updated: 2023/05/22 17:16:30 by akharraz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

SRC = $(addsuffix .cpp, main ircserv)

INC = ircserv.hpp

FLAGS = -Wall -Wextra -Werror -std=c++98

OBJS = $(SRC:.cpp=.o)
all : ${NAME}

${OBJS} : %.o: %.cpp ${INC}
	${CC} -c $(FLAGS) $< -o $@ -c

${NAME} : ${OBJS}
	${CC} ${FLAGS} ${OBJS} -o ${NAME}


clean : 
	rm -f ${OBJS}

fclean : clean
	rm -f ${NAME}

re : fclean all
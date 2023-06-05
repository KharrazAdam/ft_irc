# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/22 16:06:48 by akharraz          #+#    #+#              #
#    Updated: 2023/06/05 00:29:58 by akharraz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ./bin/ircserv
CC = c++
SRC = $(addsuffix .cpp, ircserv client main)

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
 
INC = $(addprefix ./inc/, ircserv.hpp client.hpp)

OBJS = $(addprefix obj/, $(SRC:.cpp=.o))

all: $(NAME)

obj/%.o: src/%.cpp $(INC)
	@if [ ! -d "obj" ]; then \
		mkdir obj; chmod 777 obj\
	else \
		echo chmod 777 obj; \
	fi
	$(CC) -c $(CFLAGS) -I./inc $< -o $@

$(NAME): $(OBJS)
	@if [ ! -d "bin" ]; then \
		mkdir bin; chmod 777 bin\
	else \
		echo chmod 777 bin; \
	fi
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/22 16:06:48 by akharraz          #+#    #+#              #
#    Updated: 2023/06/02 08:56:42 by akharraz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
SRC =  $(addprefix ./src/, ircserv.hpp client.hpp)

CFLAGS = -Wall -Wextra -Werror -std=c++98

INC_DIR = 
INC = $(addprefix ./inc/, ircserv.hpp client.hpp)

$(addprefix ./obj/, ircserv.hpp client.hpp)
OBJS = $(SRC:.cpp=.o)

all: $(NAME)

$(OBJS): %.o: %.cpp $(INC)
	$(CC) -c $(CFLAGS) -I./inc $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all




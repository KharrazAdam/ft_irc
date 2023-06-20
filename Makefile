# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mzridi <mzridi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/22 16:06:48 by akharraz          #+#    #+#              #
#    Updated: 2023/06/20 22:42:19 by mzridi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
SRC = $(addsuffix .cpp, ircserv client channel join main privmsg notice show mode nick user pass)

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
 
INC = $(addprefix ./inc/, ircserv.hpp channel.hpp client.hpp)

OBJS = $(SRC:.cpp=.o)

all: $(NAME)

%.o: src/%.cpp $(INC)

	$(CC) -c $(CFLAGS) -I./inc $< -o $@

$(NAME): $(OBJS)

	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

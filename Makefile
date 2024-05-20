# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmirlico <bmirlico@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/01 12:18:48 by clbernar          #+#    #+#              #
#    Updated: 2024/05/19 19:07:25 by bmirlico         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

## MAIN VARIABLES

NAME =			webserv

CPP = 			c++

FLAGS = 		-g3 -Wall -Wextra -Werror

CPP_FLAG = 		-std=c++98

RM =			rm -f

## SOURCES AND OBJECTS

SRCS =			main.cpp\
				ConfigParser.cpp\
				ServerConfig.cpp\
				Location.cpp\
				ConfigFile.cpp

OBJS =			$(SRCS:.cpp=.o)

## HEADERS

HD =			ConfigParser.hpp\
				ServerConfig.hpp\
				Location.hpp\
				ConfigFile.hpp

## RULES

all:			$(NAME)

$(NAME):		$(OBJS) $(HD)
				$(CPP) $(CPP_FLAG) $(FLAGS) $(OBJS) -o $@

%.o: %.cpp		$(HD)
				$(CPP) $(CPP_FLAG) $(FLAGS) -c $< -o $@

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY: 		all clean fclean re

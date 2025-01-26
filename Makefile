NAME = irc

SRC = main.cpp function_tools.cpp Server.cpp Client.cpp

OBJS = $(SRC:.cpp=.o)

CC =  c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

RM = rm -rf

HEADER = Server.hpp Client.hpp

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS)

fclean : clean 
	$(RM) $(NAME)

re : fclean all 
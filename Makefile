SRC = irc.cpp
OBJS = $(SRC:.cpp=.o)
CC =  c++ -std=c++98
CFLAGS = 
RM = rm -rf
NAME = irc

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS)

fclean : clean 
	$(RM) $(NAME)

re : fclean all 
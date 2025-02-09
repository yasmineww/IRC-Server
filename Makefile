
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

NAME = ircserv

SRC = main.cpp function_tools.cpp Server.cpp Client.cpp Server_Commands.cpp Channel.cpp \
	  Join.cpp \
	  Part.cpp \
	  Temp.cpp \

OBJS = $(SRC:.cpp=.o)

CC =  c++


FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

RM = rm -rf

HEADER = Server.hpp Client.hpp  Macros.hpp

all : credit $(NAME)
	@echo "$(GREEN)██████████████████████ Compiling is DONE ███████████████████████$(RESET)"
	clear


top:
	@echo "$(GREEN)█████████████████████ Making ft_irc Soup ███████████████████████$(RESET)"

$(NAME) : top $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp $(HEADER)
	@printf "$(RED)█ $(YELLOW)Compiling$(RESET) $<.\r\t\t\t\t\t\t\t..."
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "\r\t\t\t\t\t\t\t$(GREEN){DONE}$(RED) █$(RESET)"

clean :
	@echo "$(YELLOW)--> $(RESET)$(RED)Deleting files$(RESET) : " $(OBJS)
	@$(RM) $(OBJS)

fclean : clean
	@echo "$(YELLOW)--> $(RESET)$(RED)Deleting program name $(RESET): " $(NAME)
	@$(RM) $(NAME)

re : fclean all

credit:
	@echo " 			┏━━┓┏━━━┓┏━━━┓ 				"
	@echo " 			┗┫┣┛┃┏━┓┃┃┏━┓┃ 				"
	@echo " 			 ┃┃ ┃┗━┛┃┃┃ ┗┛ 				"
	@echo " 			 ┃┃ ┃┏┓┏┛┃┃ ┏┓ 				"
	@echo " 			┏┫┣┓┃┃┃┗┓┃┗━┛┃ 				"
	@echo " 			┗━━┛┗┛┗━┛┗━━━┛ 	$(YELLOW)LAYMOUNA.$(RESET)	"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "━━━━━━━━━┃ Made with $(RED)❤️$(RESET) by : \033[1;91 gyoumoukh ymakhlou mmaghri\033[m ┃━━━━━━━━━"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"





# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 10:52:26 by youmoukh          #+#    #+#              #
#    Updated: 2025/03/01 19:12:30 by ymakhlou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

NAME = ircserv

BNAME = Bot

SRC = main.cpp Server/Utils.cpp Server/Server.cpp Server/Server_Commands.cpp Server/Server_Utils.cpp Client/Client.cpp \
      Commands/Join.cpp Commands/Part.cpp Commands/Kick.cpp Commands/Invite.cpp \
	  Commands/Topic.cpp Commands/Mode.cpp Commands/Notice.cpp Commands/Quit.cpp \
	  Commands/Privmsg.cpp Channel/Channel.cpp \

BONUS = Bonus/Bot.cpp Bonus/mainBonus.cpp

HEADERS = Headers/Macros.hpp Headers/Server.hpp Headers/Client.hpp Headers/Channel.hpp

BHEADERS = Bonus/Bot.hpp

OBJS = $(SRC:.cpp=.o)

OBJB = $(BONUS:.cpp=.o)

CC =  c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

RM = rm -rf

all : credit $(NAME)
	@echo "$(GREEN)██████████████████████ Compiling is DONE ███████████████████████$(RESET)"
	clear

top:
	@echo "$(GREEN)█████████████████████ Making ft_irc 🍊 █████████████████████████$(RESET)"

$(NAME) : top $(OBJS) $(HEADERS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

${BNAME} : $(OBJB) $(BHEADERS)
	@$(CC) $(FLAGS) $(OBJB) -o $(BNAME)


%.o : %.cpp $(HEADERS)
	@printf "$(RED)█ $(YELLOW)Compiling$(RESET) $<.\r\t\t\t\t\t\t\t..."
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "\r\t\t\t\t\t\t\t$(GREEN){DONE}$(RED) █$(RESET)"

bonus : ${BNAME}

clean :
	@echo "$(YELLOW)--> $(RESET)$(RED)Deleting files$(RESET) : " $(OBJS)
	@$(RM) $(OBJS) $(OBJB)

fclean : clean
	@echo "$(YELLOW)--> $(RESET)$(RED)Deleting program name $(RESET): " $(NAME)
	@$(RM) $(NAME) $(BNAME)

re : fclean all

credit:
	@echo " 			┏━━┓┏━━━┓┏━━━┓ 				"
	@echo " 			┗┫┣┛┃┏━┓┃┃┏━┓┃ 				"
	@echo " 			 ┃┃ ┃┗━┛┃┃┃ ┗┛ 				"
	@echo " 			 ┃┃ ┃┏┓┏┛┃┃ ┏┓ 				"
	@echo " 			┏┫┣┓┃┃┃┗┓┃┗━┛┃ 				"
	@echo " 			┗━━┛┗┛┗━┛┗━━━┛ 	$(YELLOW)🍊 LAYMOUNA 🍊 $(RESET)	"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "━━━━━━━━━┃ 🍊 Made with $(RED)❤️$(RESET) by : \033[1;91 gyoumoukh ymakhlou mmaghri\033[m ┃━━━━━━━━━"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"


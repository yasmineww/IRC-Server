# define PASS 1
# define USER 2
# define NICK 3
# define MODE 4
# define JOIN 5
# define HELP 6
# define PRIVMSG 7
# define REALNAME 99
# define USERNAME 88
# define SERVERNAME 77
# define HOSTNAME 66


#define PASS_STR "PASS"
#define USER_STR "USER"
#define NICK_STR "NICK"
#define MODE_STR "MODE"
#define PRIVMSG_STR "PRIVMSG"
#define JOIN_STR "JOIN"
#define HELP_STR "HELP"

# define GREEN "\e[1;32m"
# define RESET "\e[0m"
# define RED "\e[1;91m"
# define CYAN "\e[1;36m"
# define YELLOW "\e[1;33m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"


# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <vector>
# include <unistd.h>
# include <map>
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include <cstring>
# include <cstdlib>

#include <cstring>
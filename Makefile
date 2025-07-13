NAME = webserv

CC = c++
RM = rm -rf
EC = echo
CFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDE_DIR = include
INCLUDE = -I $(INCLUDE_DIR)

SRCS_DIR = sources/
SRCS_FILES = webserv.cpp CGIHandler.cpp ConfigFileParser.cpp ConnectionHandler.cpp ErrorHandler.cpp HttpRequest.cpp HttpResponse.cpp LocationConfig.cpp RequestParser.cpp ResponseBuiler.cpp Router.cpp ServerConfig.cpp ServerManager.cpp Utils.cpp
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS = $(SRCS:.cpp=.o)


.cpp.o:
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(EC) "$(OBJS) created"
	@$(CC) $(CFLAGS) $(OBJS) $(INCLUDE) -o $(NAME)
	@$(EC) "$(NAME) created"


clean: 
	@$(RM) $(OBJS)
	@$(EC) "$(OBJS) removed"

fclean: clean
	@$(RM) $(NAME)
	@$(EC) "$(NAME) removed"

re: fclean all

run: all clean


.PHONY: all clean fclean re run

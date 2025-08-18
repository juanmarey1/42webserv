NAME = webserv

CC = c++
RM = rm -rf
EC = echo
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

INCLUDE_DIR = include
INCLUDE = -I $(INCLUDE_DIR)

SRCS_DIR = sources/
SRCS_FILES = webserv.cpp CGIHandler.cpp ConfigFileParser.cpp ConnectionHandler.cpp ErrorHandler.cpp HttpRequest.cpp HttpResponse.cpp LocationConfig.cpp RequestParser.cpp ResponseBuilder.cpp Router.cpp ServerConfig.cpp ServerManager.cpp Utils.cpp
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

TESTS_DIR = tests/
TESTS_FILES = testCGIHandler.cpp testConfigFileParser.cpp testConnectionHandler.cpp testRequestParser.cpp testResponseBuilder.cpp
TEST_SRCS = $(addprefix $(TESTS_DIR), $(TESTS_FILES))

TOTAL_TESTS = $(TEST_SRCS) $(SRCS)
TEST_OBJS = $(TOTAL_TESTS:.cpp=.o)
TEST = test


OBJS = $(SRCS:.cpp=.o)


.cpp.o:
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

all: $(NAME)

tests: $(TEST)

$(TEST): $(TEST_OBJS)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(INCLUDE) -o $(TEST)

tclean:
	@$(RM) $(TEST_OBJS) $(TEST)
toclean:
	@$(RM) $(TEST_OBJS)

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

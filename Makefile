CXX:= c++
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98

SRCS:=  main.cpp $(CONFIGURATION_PARSER_SRCS)
HEADERS:=
include CONFIGURATION_PARSER/config.mk
include SERVER/server.mk
include EXCEPTIONS/exceptions.mk
include UTILITIES/utilities.mk

OBJS:= $(SRCS:.cpp=.o) 
RM:= rm -f
NAME:= webserv

all: $(NAME)

$(NAME): $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
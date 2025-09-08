CXX:= c++
CXXFLAGS:= -Wall -Wextra -Werror -std=c++98
CONFIGURATION_PARSER_SRCS:= configurationParser/Scanner.cpp configurationParser/Token.cpp 
SRCS:=  main.cpp $(CONFIGURATION_PARSER_SRCS)

OBJS:= $(SRCS:.cpp=.o) 
HEADERS:= 
RM:= rm -f
NAME:= webserv

all: $(NAME)

$(NAME): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsuchen <tsuchen@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/23 14:04:06 by bthomas           #+#    #+#              #
#    Updated: 2024/09/30 19:14:23 by tsuchen          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
SRCDIR		=	src
OBJDIR		=	obj
CFLAGS		=	-Wall -Werror -Wextra -std=c++98 -MMD -g3
CPP			=	c++

INCS		=	-I ./include
INCS		+=	-I ./include/parser
INCS		+=	-I ./include/multiplexer
INCS		+=  -I ./include/http

SRC			=	main.cpp
SRC			+=	parser.cpp lexer.cpp json.cpp
SRC			+=	EventHandler.cpp Server.cpp ClientConnection.cpp EventHandler_cgi.cpp
SRC			+=	AHttpData.cpp Request.cpp Response.cpp

vpath %.cpp src/
vpath %.cpp src/multiplexer
vpath %.cpp src/parser
vpath %.cpp src/http

OBJS		= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

DEPS = $(OBJS:%.o=%.d)

all: $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CPP) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CPP) $(CFLAGS) $(INCS) -o $(NAME) $(OBJS)
	@echo "webserv build done!"

-include $(DEPS)

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing webserv..."
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re

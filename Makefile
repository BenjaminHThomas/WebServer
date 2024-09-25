# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/23 14:04:06 by bthomas           #+#    #+#              #
#    Updated: 2024/09/25 15:06:25 by bthomas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
SRCDIR		= src
OBJDIR		= obj
INCS		= -I ./include
SRC			= main.cpp \
				EventHandler.cpp \
				Server.cpp
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -MMD
CPP			= c++

vpath %.cpp src/ src/multiplexer

OBJS		= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

DEPS = $(OBJS:%.o=%.d)

-include $(DEPS)

all: $(OBJDIR) $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CPP) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) $(INCS) -o $(NAME) $(OBJS)
	@echo "webserv build done!"

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing webserv..."
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re

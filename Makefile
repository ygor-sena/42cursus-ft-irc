# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yde-goes <yde-goes@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 19:40:24 by gilmar            #+#    #+#              #
#    Updated: 2024/05/24 18:16:38 by yde-goes         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
##                                MINIRT                                      ##
################################################################################

NAME  := ircserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

################################################################################
##                                DIRECTORIES                                 ##
################################################################################

OBJ_DIR   := obj
#LIB_DIR   := lib
INC_DIRS  := include

# If there are subdirectories in src, add them here
SRC_DIRS  := commands
# If the line above is not enough, add the subdirectories here
#SRC_DIRS  += 
SRC_DIRS  := $(addprefix src/, $(SRC_DIRS))
SRC_DIRS  += src

vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

HEADERS := Server.hpp Client.hpp Replies.hpp Channel.hpp
# If the line above is not enough, add the headers here
#HEADERS +=

SOURCES := main.cpp Client.cpp Server.cpp Channel.cpp Invite.cpp Join.cpp
SOURCES += Kick.cpp Nick.cpp Part.cpp Privmsg.cpp Quit.cpp Topic.cpp User.cpp
SOURCES += Mode.cpp Pass.cpp


OBJS := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))

################################################################################
##                                 COMPILATION                                ##
################################################################################

CC = g++
CFLAGS  := -Wall -Werror -Wextra -std=c++98 $(addprefix -I,$(INC_DIRS))
# Library flags, such as LIBFT_DIR and MLX_DIR
LDFLAGS := #-L 
# Libraries to link, such as -lft and -lmlx
LDLIBS  := #

ifdef DEBUG
	CFLAGS += -g
else
	CFLAGS += -Ofast
endif

all: $(NAME)

run:
	./$(NAME) 12345 54321

tel:
	nc localhost 4444

r: $(NAME)
	@make $(TEST) -C tests --no-print-directory

test: $(NAME)
	@make -C tests --no-print-directory

$(NAME): $(OBJS)
	@$(LOG) "Building $@"
	@$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: %.cpp $(HEADERS) | $(OBJ_DIR)
	@$(LOG) "Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(LOG) "Creating objects directory"
	@mkdir $@

$(LIBFT):
	@make bonus -C --no-print-directory

checks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

clean:
	@$(RM) -r $(OBJS)
	@$(LOG) "Removing objects"
	@$(RM) -r $(OBJ_DIR)
	@$(LOG) "Removing objects directory"

fclean: clean
	@$(RM) -r $(NAME)
	@$(LOG) "Removing $(NAME)"

re: clean all

.PHONY: all clean fclean re



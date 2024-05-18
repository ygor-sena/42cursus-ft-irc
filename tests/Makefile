# Insert the test rules here
TESTS := server
#TESTS += 

CYAN  := \33[1;36m
RESET := \033[0m
LOG   := printf "[$(CYAN)INFO$(RESET)] %s\n"

SRC_DIR   := ../src
OBJ_DIR   := ../obj
INC_DIRS  := ../include
SRC_DIRS  := $(shell find $(SRC) -type d)

vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -exec basename {} \;)
SOURCES := $(filter-out main.cpp, $(SOURCES))
UTILS   := Utils.cpp
OBJS    := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
OBJS    += $(UTILS:.cpp=.o)

CC = g++
CFLAGS  := -Wall -Werror -Wextra -O3 $(addprefix -I,$(INC_DIRS))
#LDFLAGS := -L
LDLIBS  := -lcriterion

define run
 	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@ && ./$@ --verbose
endef

################################################################################
##                                 TESTS                                      ##
################################################################################

all: $(TESTS) clean

.PHONY: server
server: $(OBJS) TestServer.cpp
	@$(call run, "$^")

# Insert the test rules here, follwing the example above.

# Makefile rules to clean stdin/stdout/stderr test's files
clean:
	@$(RM) -r $(TESTS)
	@$(RM) -r $(UTILS:.cpp=.o)
	@$(LOG) "Removing objects"

fclean: clean

re: fclean all

.PHONY: all clean fclean re

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caalbert <caalbert@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 19:40:24 by gilmar            #+#    #+#              #
#    Updated: 2024/05/12 00:30:08 by caalbert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++

CFLAGS = -std=c++98

TARGET = ircserv

SRCS = main.cpp Client.cpp Server.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

run:
	./$(TARGET) 12345 54321

test:
	telnet localhost 12345

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
		$(CC) $(CFLAGS) -c -o $@ $<

clean:
		$(RM) $(TARGET) $(OBJS)

.PHONY: all clean
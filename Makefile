# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/23 19:40:24 by gilmar            #+#    #+#              #
#    Updated: 2024/04/23 20:08:53 by gilmar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++

CFLAGS = -std=c++98 -Wall

TARGET = server

SRCS = main.cpp

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
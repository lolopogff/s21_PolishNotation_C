CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRCS = plotting.c stack.c validation.c dij.c graph.c
TARGET = ../build/graph

all : $(TARGET)


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean : 
	rm -rf $(TARGET)

rebuild : clean all

.PHONY: all clean rebuild
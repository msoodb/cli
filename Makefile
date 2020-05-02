CC = gcc
CFLAGS = -c -Wall -Wextra -pedantic #-std=c99
LDFLAGS = -g
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = cli

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

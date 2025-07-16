CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c11 -Iinclude -pthread

SRC := $(wildcard src/*.c src/storage/*.c src/parser/*.c src/planner/*.c src/executor/*.c src/thread/*.c src/security/*.c)
OBJ := $(SRC:.c=.o)
TARGET := db



$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean

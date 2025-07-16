CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c11 -Iinclude

SRC := $(wildcard src/*.c src/storage/*.c)
OBJ := $(SRC:.c=.o)
TARGET := db



$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean

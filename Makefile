CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11 -Iinclude

TARGET = interpreter

SRCS = $(wildcard src/*.c)

all : $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $^ $(CFLAGS) -o $(TARGET)

clean:
	rm $(TARGET)

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -g -fsanitize=address -D_POSIX_C_SOURCE=200809L
LDFLAGS = `pkg-config --cflags --libs sdl3` -fsanitize=address -lm

# List your source files here (add more as needed)
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = mygame

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run




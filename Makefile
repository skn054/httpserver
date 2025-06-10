# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99

# The final executable name
TARGET = server

# All source files
SOURCES = server.c http_parser.c filehandler.c

# Object files are derived from source files
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile a .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rule to run the server
run: $(TARGET)
	./$(TARGET)
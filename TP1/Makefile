CC = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -Wextra -I. -Iinclude -lm

# List of source files
SOURCES = chenillard.c \

# List of object files to be generated
OBJECTS = $(SOURCES:.c=.o)

# Output executable name
EXECUTABLE = main
REMOTE_TARGET = root@192.168.88.84:~/Document

# Default target
all: $(EXECUTABLE) copy_remote

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to copy the executable to a remote location using scp
copy_remote: $(EXECUTABLE)
	scp $(EXECUTABLE) $(REMOTE_TARGET)

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

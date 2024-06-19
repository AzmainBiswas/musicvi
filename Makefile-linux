INCLUDE = ./include/
RAYLIB_INCLUDE = ./raylib/include/
RAYLIB_LIB = ./raylib/lib/
CFLAGS = -Wall -Wextra -pedantic -ggdb -I$(INCLUDE) -I$(RAYLIB_INCLUDE)
LDFLAGS = -L$(RAYLIB_LIB) -Wl,-rpath=$(RAYLIB_LIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = musicvi
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	$(RM) *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

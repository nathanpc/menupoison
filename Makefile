CC = gcc
CXXFLAGS = -Wall -std=gnu99
#LDFLAGS = -pthread
LIBS = -lcdk -lncurses

PROJECT = menupoison

HEADERS = src/xdg_menu.h
OBJECTS = src/main.o src/xdg_menu.o

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

debug: CXXFLAGS += -g3 -DDEBUG
debug: $(PROJECT)

%.o: %.c $(HEADERS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o
	rm -rf $(PROJECT)

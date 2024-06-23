CC = g++
SRCS = src/main.cpp src/one-way-tile.cpp src/player.cpp src/solid-tile.cpp
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: $(SRCS)
	$(CC) $(SRCS) $(LIBS) -o main

clean:
	rm -f main

run: main
	./main

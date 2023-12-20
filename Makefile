CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: consolol-serpent

consolol-serpent: main.cpp
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f consolol-serpent

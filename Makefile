CC = g++
CFLAGS = -Wall -Wextra -std=c++11

ifeq ($(OS),Windows_NT)
    TARGET = consolol-serpent.exe
    RM = del /Q
else
    TARGET = consolol-serpent
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET)

clean:
	$(RM) $(TARGET)

# Makefile for Snake Game

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lncurses

TARGET = snake

all: $(TARGET)

$(TARGET): snake.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) snake.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

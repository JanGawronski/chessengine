# Makefile for chess engine project

CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2

# List of source files
SRCS = main.cpp boardManipulation.cpp evaluation.cpp minmax.cpp moveGeneration.cpp transpositionTable.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = chessengine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
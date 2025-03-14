CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O3 -g

SRCS = *.cpp

TARGET = chessengine

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
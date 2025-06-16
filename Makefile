CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g

SOURCES = src/main.cpp src/lexer.cpp src/parser.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = calculator

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
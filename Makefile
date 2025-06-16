CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g

SOURCES = main.cpp lexer.cpp parser.cpp
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
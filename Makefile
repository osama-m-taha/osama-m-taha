CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = aes_demo
EXAMPLE = aes_example
SOURCES = main.cpp aes.cpp
EXAMPLE_SOURCES = example.cpp aes.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:.cpp=.o)

.PHONY: all clean test

all: $(TARGET) $(EXAMPLE)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(EXAMPLE): $(EXAMPLE_OBJECTS)
	$(CXX) $(EXAMPLE_OBJECTS) -o $(EXAMPLE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXAMPLE_OBJECTS) $(TARGET) $(EXAMPLE)

test: $(TARGET)
	./$(TARGET)

# Debug version
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release version with optimizations
release: CXXFLAGS += -DNDEBUG
release: $(TARGET)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = aes_demo
SOURCES = aes.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Running AES encryption/decryption tests..."
	./$(TARGET)

.PHONY: all clean run test
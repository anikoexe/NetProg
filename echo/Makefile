CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = echo_client
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean

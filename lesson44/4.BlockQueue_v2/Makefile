CXX = g++
CXXFLAGS = -Wall -std=c++11 -pthread
TARGET = main
SRC = main.cc

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

.PHONY: clean run

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
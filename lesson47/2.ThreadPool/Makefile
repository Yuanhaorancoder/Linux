CC = g++
CFLAGS = -std=c++17 -Wall -o
LDFLAGS = -pthread
TARGET = ThreadPool

$(TARGET): Main.cc
	$(CC) $(CFLAGS) $@ Main.cc

clean:
	rm -f $(TARGET)

CC = g++
CFLAGS = -std=c++11 -Wall -c
LDFLAGS = -pthread
TARGET = ThreadPool
OBJS = Main.o

all: $(OBJS)

Main.o: Main.cc ThreadPool.hpp
	$(CC) $(CFLAGS) Main.cc

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

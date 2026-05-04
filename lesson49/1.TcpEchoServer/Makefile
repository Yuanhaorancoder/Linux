CC = g++
CXXFLAGS = -std=c++17 -Wall

TARGETS = TcpEchoServer TcpEchoClient

all: $(TARGETS)

TcpEchoServer: TcpEchoServer.cc TcpEchoServer.hpp Logger.hpp Mutex.hpp
	$(CC) $(CXXFLAGS) -o $@ TcpEchoServer.cc

TcpEchoClient: TcpEchoClient.cc
	$(CC) $(CXXFLAGS) -o $@ TcpEchoClient.cc

clean:
	rm -f $(TARGETS)

.PHONY: all clean
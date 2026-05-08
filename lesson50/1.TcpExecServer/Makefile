CC = g++
CXXFLAGS = -std=c++17 -Wall

TARGETS = ssh_server ssh_client

all: $(TARGETS)

ssh_server: TcpExecServer.cc
	$(CC) $(CXXFLAGS) -o $@ TcpExecServer.cc

ssh_client: TcpEchoClient.cc
	$(CC) $(CXXFLAGS) -o $@ TcpEchoClient.cc -static

clean:
	rm -f $(TARGETS)

.PHONY: all clean
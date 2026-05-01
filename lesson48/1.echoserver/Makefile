CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGETS = udpEchoServer udpEchoClient

all: $(TARGETS)

udpEchoServer: UdpEchoServer.cc
	$(CXX) $(CXXFLAGS) -o udpEchoServer UdpEchoServer.cc

udpEchoClient: UdpEchoClient.cc
	$(CXX) $(CXXFLAGS) -o udpEchoClient UdpEchoClient.cc -static

clean:
	rm -f $(TARGETS)

.PHONY: all clean
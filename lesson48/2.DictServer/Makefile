.PHONY: all clean

CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: DictServer DictClient

DictServer: DictServer.cc UdpServer.hpp Logger.hpp Mutex.hpp
	$(CXX) $(CXXFLAGS) -o DictServer DictServer.cc

DictClient: DictClient.cc
	$(CXX) $(CXXFLAGS) -o DictClient DictClient.cc

clean:
	rm -f DictServer DictClient
all:Writer Reader
Reader:Reader.cc
	g++ -o $@ $^ -std=c++11
Writer:Writer.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f Writer Reader
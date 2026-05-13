all: client_cal online_cal_server
.PHONY:all

client_cal:OnlineCalClient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp
online_cal_server: OnlineCalServer.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

.PHONY:clean
clean:
	rm -f client_cal online_cal_server
CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/ConnectionHandler.o bin/StompBookClubClient.o bin/StompMessage.o bin/main.o $(LDFLAGS)

StompBookClubClient: bin/ConnectionHandler.o bin/StompBookClubClient.o bin/StompMessage.o bin/main.o

bin/main.o: src/main.cpp
	g++ -pthread $(CFLAGS) -o bin/main.o src/main.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	g++ -pthread $(CFLAGS) -o bin/StompBookClubClient.o src/StompBookClubClient.cpp

bin/StompMessage.o: src/StompMessage.cpp
	g++ -pthread $(CFLAGS) -o bin/StompMessage.o src/StompMessage.cpp



clean:
	rm -f bin/*

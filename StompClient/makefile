CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: StompBookClubClient

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
StompBookClubClient: bin/StompBookClubClient.o bin/ConnectionHandler.o bin/StompMessage.o
	@echo 'Building target: StompBookClubClient'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/hello bin/StompBookClubClient.o bin/Run.o $(LFLAGS)
	@echo 'Finished building target: hello'
	@echo ' '

# Depends on the source and header files
bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StompBookClubClient.o src/StompBookClubClient.cpp

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/StompMessage.o: src/StompMessage.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StompMessage.o src/StompMessage.cpp

#Clean the build directory
clean: 
	rm -f bin/*

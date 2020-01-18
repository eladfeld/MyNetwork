#include <iostream>
#include <thread>
#include "include/StompBookClubClient.h"


using namespace std;

void getUserInput(){
    while(1) {
        string in;
        getline(std::cin, in);
        cout << "user entered : " << in;
    }
}

int main() {
    StompBookClubClient client;
    client.readFromUser();
    //thread userInput(std::mem_fun(&StompBookClubClient::readFromUser),client);
    //userInput.join();

    return 0;
}

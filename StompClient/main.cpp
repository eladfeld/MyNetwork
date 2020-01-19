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
    return 0;
}

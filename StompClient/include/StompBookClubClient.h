//
// Created by elad on 13/01/2020.
//

#ifndef STOMPCLIENT_STOMPBOOKCLUBCLIENT_H
#define STOMPCLIENT_STOMPBOOKCLUBCLIENT_H

#include <string>
#include <iostream>
#include <stack>
#include "ConnectionHandler.h"
#include "StompMessage.h"

using namespace std;

class StompBookClubClient{
private:
    ConnectionHandler handler;
    bool shouldTerminate;
    unordered_map<string, StompMessage> receipts;
    unordered_map<string,vector<string>> booksByTopic;
    string name;
    unordered_map<string,std::stack<string>> bookLenderTrace;
    vector<string> requestedBooks;
    void handleStompMessage(StompMessage message);
    void handleRecipt(StompMessage message);
    void handleMessage(StompMessage message);
    void handleError(StompMessage message);
    void borrowBook(StompMessage message);



public:
    void login(int port, string login, string passcode);
    void disconnect();
    void subscribe(string channel);
    void unsubscribe(string channel);
    void addBook(string channel, string bookName);
    void handleBookReturn(StompMessage message);
    void bookStatus(string channel);
    void readFromSocket();
    void readFromUser();
    void checkForBook(StompMessage message);
    void lendBook(StompMessage message);



};

#endif //STOMPCLIENT_STOMPBOOKCLUBCLIENT_H

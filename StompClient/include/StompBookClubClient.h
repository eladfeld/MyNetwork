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
#include <mutex>
using namespace std;

class StompBookClubClient{
private:
    mutex lock_booksByTopic;
    mutex lock_bookLenderTrace;
    ConnectionHandler handler;//<<------------------------------not thread safe
    bool shouldTerminate;
    unordered_map<string, StompMessage> receipts; //<<----------not thread safe
    unordered_map<string,list<string>> booksByTopic; //<<-------not thread safe
    string name;
        unordered_map<string,std::stack<string>> bookLenderTrace;//<<not thread safe
    vector<string> requestedBooks;
    int nextReceiptId;
    int nextSubscriptionId;
    unordered_map<string,int> subscriptionIds;
    void handleStompMessage(StompMessage message);
    void handleRecipt(StompMessage message);
    void handleMessage(StompMessage message);
    void handleError(StompMessage message);
    void borrowBook(StompMessage message);
    void handleCommand(string basicString);
    void requestBorrowBook(string topic, string bookName);
    string getBookName(vector<string> command);
    void returnBook(string topic, string bookName);
    void requestStatus(string topic);
    void login(string server, string login, string passcode);
    void disconnect();
    void subscribe(string channel);
    void unsubscribe(string channel);
    void addBook(string channel, string bookName);
    void handleBookReturn(StompMessage message);
    void bookStatus(string channel);
    void checkForBook(StompMessage message);
    void lendBook(StompMessage message);
    void run();
    bool bookNotExist(string topic, string bookName);



public:
    StompBookClubClient();
    ~StompBookClubClient();
    void readFromSocket();
    void readFromUser();


};

#endif //STOMPCLIENT_STOMPBOOKCLUBCLIENT_H

//
// Created by elad on 13/01/2020.
//

#include <thread>
#include "../include/StompBookClubClient.h"

StompBookClubClient::StompBookClubClient() :shouldTerminate(true), nextReceiptId(1), nextSubscriptionId(1){

}

StompBookClubClient::~StompBookClubClient() = default;

void StompBookClubClient::readFromSocket() {
    shouldTerminate=false;
    while (!shouldTerminate) {
        string message = handler.Read();
        if (message != "") {
            StompMessage stompy =StompMessage::parse(message);
            handleStompMessage(stompy);
            string topic = stompy.getHeaders()["destination"];
            if(topic != ""){
                cout<<topic<<" : "<<stompy.getBody()<<endl;
            }
        }
    }
    handler.Close();
    cout<<"logged out"<<endl;
}

void StompBookClubClient::readFromUser() {
    string command;
    while (true) {
        getline(std::cin, command);
        if(command == "s")command  = "login 127.0.0.1:7777 eladBaby kapara";
        if(command == "t")command  = "login 127.0.0.1:7777 tBaby kapara";
        if(command == "r")command  = "login 127.0.0.1:7777 blackMan kapara";
        handleCommand(command);
    }
}

// REGION Keyboard command handling

void StompBookClubClient::handleCommand(string command) {
    vector<string> words = StompMessage::split(command, ' ');
    if(words.size()==0){
        cout << "illegal (empty) command! try again" << endl;
        return;
    }
    string commandType = words.at(0);
    if (commandType == "login" & (words.size() == 4))login(words.at(1), words.at(2), words.at(3));
    else if (commandType == "join" & (words.size() == 2))subscribe(words.at(1));
    else if (commandType == "exit" & (words.size() == 2))unsubscribe(words.at(1));
    else if (commandType == "borrow" & (words.size() >= 3)) requestBorrowBook(words.at(1), getBookName(words));
    else if (commandType == "add" & (words.size() >= 3)) addBook(words.at(1), getBookName(words));
    else if (commandType == "return" & (words.size() >= 3)) returnBook(words.at(1), getBookName(words));
    else if (commandType == "status" & (words.size() == 2)) requestStatus(words.at(1));
    else if (commandType == "logout" & (words.size() == 1)) disconnect();
    else cout << "illegal command! try again" << endl;

}

void StompBookClubClient::subscribe(string channel) {
    unordered_map<string, string> headers;
    headers["destination"] = channel;
    headers["receipt"] = to_string(nextReceiptId);
    headers["id"]=to_string(nextSubscriptionId++);
    StompMessage msg("SUBSCRIBE", headers, "\0");
    receipts[to_string(nextReceiptId++)] = msg;
    handler.Send(msg.toString());
}

void StompBookClubClient::unsubscribe(string channel) {
    unordered_map<string, string> headers;
    headers["destination"] = channel;
    headers["id"] = to_string(subscriptionIds[channel]);
    headers["receipt"] = to_string(nextReceiptId);
    StompMessage msg("UNSUBSCRIBE", headers, "\0");
    receipts[to_string(nextReceiptId++)] = msg;
    handler.Send(msg.toString());
}

void StompBookClubClient::requestBorrowBook(string topic, string bookName) {
    if (bookNotExist(topic, bookName)) {
        unordered_map<string, string> headers;
        headers["destination"] = topic;
        headers["bookName"] = bookName;
        requestedBooks.push_back(bookName);
        StompMessage msg("SEND", headers, name + " wish to borrow " + bookName + "\0");
        handler.Send(msg.toString());
    }else{
        cout<<"you already have the book: "<<bookName<<endl;
    }
}

void StompBookClubClient::login(string server, string login, string passcode) {
    if( !shouldTerminate){
        cout<<" you are already logged in"<<endl;
        return;
    }
    vector<string> hostAndPort = StompMessage::split(server, ':');
    if (hostAndPort.size() != 2)cout << "server must be in host:port format"<<endl;
    else {
        string host = hostAndPort.at(0);
        string port = hostAndPort.at(1);
        handler.setHostAndPort(host,stoi(port));
        if (handler.Connect()) {
            thread socketInput(&StompBookClubClient::readFromSocket, this);
            socketInput.detach(); //might need this line, it was in the stackOverflow page
            unordered_map<string, string> headers;
            headers["host"] = host;
            headers["login"] = login;
            headers["passcode"] = passcode;
            StompMessage connectMsg("CONNECT", headers, ""+'\0');
            try {
                handler.Send(connectMsg.toString());
            }catch (std::exception& e){
                cout<< e.what()<<endl;
            }
            name = login;
        }
    }
}

void StompBookClubClient::requestStatus(string topic) {
    unordered_map<string, string> headers;
    headers["destination"] = topic;
    StompMessage msg("SEND", headers, "book status\0");
    handler.Send(msg.toString());
}

void StompBookClubClient::disconnect() {
    unordered_map<string, string> headers;
    headers["receipt"] = to_string(nextReceiptId);
    StompMessage msg("DISCONNECT", headers, "\0");
    receipts[to_string(nextReceiptId++)] = msg;
    handler.Send(msg.toString());
}


void StompBookClubClient::addBook(string topic, string bookName) {
    if(bookNotExist(topic, bookName)) {
        lock_booksByTopic.lock();
        booksByTopic[topic].push_back(bookName);
        lock_booksByTopic.unlock();
        unordered_map <string, string> headers;
        headers["destination"] = topic;
        string body = name + " has added the book " + bookName + "\0";
        handler.Send(StompMessage("SEND", headers, body).toString());
    }else{
        cout<<"already have this book! : " + bookName<<endl;
    }
}

void StompBookClubClient::returnBook(string topic, string bookName) {
    lock_booksByTopic.lock();
    booksByTopic[topic].remove(bookName);
    lock_booksByTopic.unlock();
    string lenderName;
    stack<string> books = bookLenderTrace[bookName];
    lock_bookLenderTrace.lock();
    lenderName = books.top();
    books.pop();
    lock_bookLenderTrace.unlock();
    unordered_map<string, string> headers;
    headers["destination"] = topic;
    headers["bookName"]=bookName;
    headers["owner"]=lenderName;
    string body = "Returning " + bookName + " to " + lenderName + "\0";
    handler.Send(StompMessage("SEND", headers, body).toString());
}

string StompBookClubClient::getBookName(vector<string> command) {
    string bookName;
    for (int i = 2; i < command.size(); i++)bookName += command.at(i) + " ";
    bookName = bookName.substr(0, bookName.size() - 1);
    return bookName;
}

//END_REGION Keyboard command handling










//REGION handle StompMessages from server
void StompBookClubClient::handleStompMessage(StompMessage message) {
    string command = message.getCommand();
    if (command == "CONNECTED")cout << "Login successful" << endl;
    else if (command == "RECEIPT")handleRecipt(message);
    else if (command == "MESSAGE")handleMessage(message);
    else if (command == "ERROR")handleError(message);
}

void StompBookClubClient::handleError(StompMessage message) {
    cout << message.toString();
}

void StompBookClubClient::handleRecipt(StompMessage message) {
    //
    //print receipt??
    //
    string receiptId = message.getHeaders()["receipt-id"];
    StompMessage origMsg = receipts[receiptId];
    string command = origMsg.getCommand();
    if (command == "DISCONNECT") {
        shouldTerminate = true;
        booksByTopic.clear();
    } else if (command == "SUBSCRIBE") {
        string genre = origMsg.getHeaders()["destination"];
        cout << "Joined club " << genre << endl;

    } else if (command == "UNSUBSCRIBE") {
        string genre = origMsg.getHeaders()["destination"];
        cout << "Exited club " << genre << endl;
    }
    //
    //remove receipt thread-safely
    //
}

void StompBookClubClient::handleMessage(StompMessage message) {
    string body = message.getBody();
    if (body.find("book status") != string::npos) {
        string topic = message.getHeaders()["destination"];
        bookStatus(topic);
    } else if (body.find(" added the book ") != string::npos) {
        cout << body << endl;
    } else if (body.find(" wish to borrow ") != string::npos) {
        checkForBook(message);
    } else if (body.find("Taking ") != string::npos) {
        lendBook(message);
    } else if (body.find("Returning ") != string::npos) {
        handleBookReturn(message);
    } else if (body.find(" has ") != string::npos) {
        borrowBook(message);
    }
}
//END_REGION handle StompMessages from server










//region handle "MESSAGE"


void StompBookClubClient::checkForBook(StompMessage message) {
    //retrive info from message the message
    string body = message.getBody();
    string bookName = message.getHeaders()["bookName"];
    string topic = message.getHeaders()["destination"];
    list<string> books = booksByTopic[topic];
    //;look for the book in the inventory
    for (string book : books) {
        if (book == bookName) {
            //if the book have found then send a message to topic
            string title = "SEND";
            unordered_map<string, string> headers;
            headers["destination"] = topic;
            headers["owner"] = name;
            headers["bookName"] = bookName;
            string body = name + " has " + bookName;
            StompMessage response(title, headers, body);
            handler.Send(response.toString());
            break;
        }
    }
}

//response to taking
void StompBookClubClient::lendBook(StompMessage message) {
    string body = message.getBody();
    cout << body << endl;
    if (message.getHeaders()["owner"] == name) {
        string book = message.getHeaders()["bookName"];
        string topic = message.getHeaders()["destination"];
        booksByTopic[topic].remove(book);
    }
}

//response to someone "has" book
void StompBookClubClient::borrowBook(StompMessage message) {
    //init message info

    string bookName = message.getHeaders()["bookName"];
    string owner = message.getHeaders()["owner"];
    string topic = message.getHeaders()["destination"];
    //check if the user had requested for this book
    for (unsigned int i = 0; i < requestedBooks.size(); i++) {
        string book = requestedBooks.at(i);
        if (book == bookName) {
            //if it did then acquire this book and delete it from your requested book list and remember who gave it to you
            booksByTopic[topic].push_back(bookName);
            unordered_map<string, string> headers;
            headers["destination"] = topic;
            headers["owner"] = owner;
            headers["bookName"] = bookName;
            string body = "Taking " + bookName + " from " + owner;
            StompMessage msg("SEND", headers, body);
            requestedBooks.erase(requestedBooks.begin() + i);
            bookLenderTrace[bookName].push(owner);
            handler.Send(msg.toString());
        }
    }
}
//response to returning book
void StompBookClubClient::handleBookReturn(StompMessage message) {
    string topic = message.getHeaders()["destination"];
    //cout << topic << " : " << message.getBody() << endl;
    if (message.getHeaders()["owner"] == name) {//owner refers to the lender
        string bookName = message.getHeaders()["bookName"];
        booksByTopic[topic].push_back(bookName);
    }
}

void StompBookClubClient::bookStatus(string channel) {
    list<string> books = booksByTopic[channel];
    string body = name + " : ";
    if (!books.empty()) {
        for (string book : books) {
            body.append(book + ",");
        }
        body = body.substr(0, body.length() - 1);
        unordered_map<string, string> header;
        header["destination"] = channel;
        StompMessage message("SEND", header, body);
        handler.Send(message.toString());
    }
}

bool StompBookClubClient::bookNotExist(string topic, string bookName) {
   list<string> books = booksByTopic[topic];
   for(string book : books){
       if(book==bookName)return false;
   }
    return true;
}
//endregion


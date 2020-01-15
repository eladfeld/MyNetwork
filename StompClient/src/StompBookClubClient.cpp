//
// Created by elad on 13/01/2020.
//

#include "../include/StompBookClubClient.h"

void StompBookClubClient::subscribe(string channel) {

}

void StompBookClubClient::unsubscribe(string channel) {

}




void StompBookClubClient::login(int port, string login, string passcode) {

}



void StompBookClubClient::readFromSocket() {
    while (!shouldTerminate) {
        string message = handler.Read();
        if (message != "") {
            cout<<message<<endl;//FOR TESTING PUPOSES
            handleMessage(StompMessage::parse(message));
        }
    }
    handler.Close();
}

void StompBookClubClient::readFromUser() {
    string command;
    getline(cin,command);


}





//handle StompMessage
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
    if (body == "book status") {
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
//end region handle StompMessage










//region handle "MESSAGE"
void StompBookClubClient::checkForBook(StompMessage message) {
    //retrive info from message the message
    string body = message.getBody();
    string keyWords = " wish to borrow ";
    unsigned int indexOfBookName = body.find(keyWords) + keyWords.length();
    string bookName = body.substr(indexOfBookName);
    string topic = message.getHeaders()["destination"];
    cout << topic << " : " << body << endl;
    vector<string> books = booksByTopic[topic];
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

void StompBookClubClient::lendBook(StompMessage message) {
    string body = message.getBody();
    cout << body << endl;
    if (message.getHeaders()["owner"] == name) {
        string book = message.getHeaders()["bookName"];
        string topic = message.getHeaders()["topic"];
        vector<string> books = booksByTopic[topic];
        int position = -1;
        for (unsigned int i = 0; i < books.size(); i++)if (books.at(i) == book)position = i;
        books.erase(books.begin()+position);
    }

}

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

void StompBookClubClient::handleBookReturn(StompMessage message) {
    string topic = message.getHeaders()["destination"];
    cout<<topic<<" : "<<message.getBody()<<endl;
    if(message.getHeaders()["owner"]==name){//owner refers to the lender
        string bookName = message.getHeaders()["bookName"];
        booksByTopic[topic].push_back(bookName);
    }
}

void StompBookClubClient::bookStatus(string channel) {
    vector<string> books = booksByTopic[channel];
    string body = name + " : ";
    if (!books.empty()) {
        for (string book : books) {
            body.append(book + ",");
        }
        body.substr(0, body.length() - 1);
        unordered_map<string, string> header;
        header["destination"] = channel;
        StompMessage message("SEND", header, body);
        handler.Send(message.toString());
    }
}
//endregion


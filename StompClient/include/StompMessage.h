//
// Created by elad on 13/01/2020.
//

#ifndef STOMPCLIENT_STOMPMESSAGE_H
#define STOMPCLIENT_STOMPMESSAGE_H



#include <string>
#include <unordered_map>

using namespace std;


class StompMessage{
private:
    string command;
    unordered_map<string, string> headers;
    string body;
public:
    StompMessage(string cmd, unordered_map<string, string> hdrs, string bdy);
    string toString();
    static StompMessage parse(string msg);
    string getCommand();
    unordered_map getHeaders();
    string getBody();



};

#endif //STOMPCLIENT_STOMPMESSAGE_H

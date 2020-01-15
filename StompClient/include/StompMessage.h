//
// Created by elad on 13/01/2020.
//

#ifndef STOMPCLIENT_STOMPMESSAGE_H
#define STOMPCLIENT_STOMPMESSAGE_H



#include <string>
#include <unordered_map>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace boost;
using namespace std;


class StompMessage{
private:
    string command;
    unordered_map<string, string> headers;
    string body;
    static vector<string> split(string s, char delimiter);
public:
    StompMessage(string cmd, unordered_map<string, string> hdrs, string bdy);
    StompMessage();
    string toString();
    static StompMessage parse(string msg);
    string getCommand();
    unordered_map<string, string> getHeaders();
    string getBody();



};

#endif //STOMPCLIENT_STOMPMESSAGE_H

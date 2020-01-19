//
// Created by elad on 13/01/2020.
//


#include <sstream>
#include <iostream>
#include "../include/StompMessage.h"

using namespace boost;

StompMessage::StompMessage(string cmd, unordered_map<string, string> hdrs, string bdy):
        command(cmd), headers(hdrs),body(bdy){}

StompMessage::StompMessage(): command(),headers(),body(){}

StompMessage  StompMessage::parse(string msg) {
    StompMessage output;
    vector<string> v = split(msg, '\n');
    output.command = v.at(0);
    unsigned int index = 1;
    while(index < v.size() && v.at(index) != ""){
        vector<string> header = split(v.at(index), ':');
        if(header.size() == 2) output.headers[header.at(0)] = header.at(1);
        else std::cout<< "illegal header !!"<<v.at(index)<<endl;
        index++;
    }
    while(index < v.size())
        output.body += v.at(index++) + "\n";
    output.body += '\0';
    return output;
}

string StompMessage::toString() {
    string output = command + "\n";
    for(auto &header : headers)
        output += header.first + ":" + header.second + "\n";
    output += "\n" + body+'\0';
    return output;
}


string StompMessage::getCommand() {
    return command;
}

unordered_map<string, string> StompMessage::getHeaders() {
    return headers;
}

string StompMessage::getBody() {
    if(body[0] == '\n')return body.substr(1, body.size()-3);
    else return body.substr(0, body.size()-2);

}
vector<string> StompMessage::split(string s, char delimiter) {
    std::stringstream stream(s);
    vector<string> output;
    std::string token;
    while(getline(stream ,token, delimiter)){
        output.push_back(token);
    }
    return output;
}


//
// Created by elad on 13/01/2020.
//


#include "../include/StompMessage.h"


string StompMessage::toString() {
    return nullptr;
}

StompMessage  StompMessage::parse(string msg) {
    return StompMessage();
}

string StompMessage::getCommand() {
    return command;
}

unordered_map<string, string> StompMessage::getHeaders() {
    return headers;
}

string StompMessage::getBody() {
    return body;
}

StompMessage::StompMessage(string cmd, unordered_map<string, string> hdrs, string bdy):
command(cmd), headers(hdrs),body(bdy){}


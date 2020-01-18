//
// Created by elad on 13/01/2020.
//

#include <iostream>
#include "../include/ConnectionHandler.h"

using namespace boost;

ConnectionHandler::ConnectionHandler()
        : ioService(), sock(ioService){}

ConnectionHandler::~ConnectionHandler() {

}

//read bytes from socket until null char and send corresponding string to client
string ConnectionHandler::Read() {
    asio::streambuf buf;
    asio::read_until(sock, buf, '\0');
    return boost::asio::buffer_cast<const char *>(buf.data());
}

void ConnectionHandler::Send(string message) {
    int bytesToWrite=message.length();
    char msg[bytesToWrite];
    strcpy(msg, message.c_str());
    int tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
            tmp += sock.write_some(boost::asio::buffer(msg + tmp, bytesToWrite - tmp), error);
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return; //false;
    }
}


void ConnectionHandler::Close() {
    sock.close();
}

bool ConnectionHandler::Connect() {
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port); // the server endpoint
        boost::system::error_code error;
        sock.connect(endpoint, error);
        if (error) {
            cout << "error" << endl;
            throw boost::system::system_error(error);
        }
    }
    catch (std::exception &e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

void ConnectionHandler::setHostAndPort(string _host, int _port) {
    host = _host;
    port = _port;
}

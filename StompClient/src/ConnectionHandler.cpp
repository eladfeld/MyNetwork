//
// Created by elad on 13/01/2020.
//

#include <iostream>
#include "../include/ConnectionHandler.h"

using namespace boost;

ConnectionHandler::ConnectionHandler(short port, string host)
: port(port), host(host), ioService(),sock(ioService), shouldTerminate(false)
{}

ConnectionHandler::~ConnectionHandler() {

}
//read bytes from socket until null char and send corresponding string to client
string ConnectionHandler::Read() {
        asio::streambuf buf;
        asio::read_until( sock, buf, '\0' );
        return boost::asio::buffer_cast<const char*>(buf.data());
}

void ConnectionHandler::Send(string message) {
    //const string msg = message + "\0"; //append null char??
    asio::write( sock, asio::buffer(message) );
}


void ConnectionHandler::Close() {
    shouldTerminate = true;
}

bool ConnectionHandler::Connect() {
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port); // the server endpoint
        boost::system::error_code error;
        sock.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

void ConnectionHandler::setHostAndPort(string _host, int _port) {
    host=_host;
    port=_port;
}

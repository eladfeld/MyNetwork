//
// Created by elad on 13/01/2020.
//

#ifndef STOMPCLIENT_CONNECTIONHANDLER_H
#define STOMPCLIENT_CONNECTIONHANDLER_H

#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

class ConnectionHandler{
private:
    const short port;
    const string host;
    boost::asio::io_service ioService;
    tcp::socket sock;
    bool shouldTerminate;

public:
    ConnectionHandler(short port, string host);
    ~ConnectionHandler();

    void Send(string message);
    string Read();
    void Close();


};

#endif //STOMPCLIENT_CONNECTIONHANDLER_H

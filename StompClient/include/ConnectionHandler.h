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
    short port;
    const string host;
    boost::asio::io_service ioService;
    tcp::socket sock;
    bool shouldTerminate;

public:
    ConnectionHandler(short port, string host);
    ~ConnectionHandler();

    void Send(string message);
    bool Connect();
    string Read();
    void Close();
    void setHostAndPort(string _host,int _port);


};

#endif //STOMPCLIENT_CONNECTIONHANDLER_H

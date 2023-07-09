#include "client.h"

Client::Client(int socket) : socket(socket)
{
    // Generate a unique client ID based on the socket address
    clientID = std::to_string(socket);
    connectionTime = std::chrono::system_clock::now();
}

Client::~Client()
{

}

int Client::getSocket() const
{
    return socket;
}

const std::string& Client::getClientID() const
{
    return clientID;
}

const std::chrono::system_clock::time_point& Client::getConnectionTime() const
{
    return connectionTime;
}

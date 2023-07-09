#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <chrono>

class Client {

public:
    Client(int socket);
    ~Client();

    int getSocket() const;
    const std::string& getClientID() const;
    const std::chrono::system_clock::time_point& getConnectionTime() const;


private:
    int socket;
    std::string clientID;
    std::chrono::system_clock::time_point connectionTime;
};

#endif  // CLIENT_H

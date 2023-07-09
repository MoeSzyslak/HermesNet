#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <atomic>
#include <mutex>
#include "client.h"
#include "constants.h"

class Server {
public:
    Server();
    ~Server();

    void start();
    void stop();

private:
    std::vector<Client*> connectedClients;
    std::mutex connectedClientsMutex;
    std::atomic<bool> isRunning;

    void handleClient(Client* client);
    void broadcastClientCount();
    std::string generateUniqueID();
};

#endif  // SERVER_H



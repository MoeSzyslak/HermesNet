#include "server.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <algorithm>
#include <set>


Server::Server() : isRunning(false) {}

Server::~Server()
{
    stop();
}

void Server::start()
{
    isRunning = true;

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    //Set up the server address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    //Bind the socket to the specified address and port
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind to address" << std::endl;
        close(serverSocket);
        return;
    }

    //Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1)
    {
        std::cerr << "Failed to listen for connections" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Server started on port " << PORT << std::endl;

    //Accept incoming connections and spawn threads to handle clients
    while (isRunning)
    {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        //Accept a new client connection
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        //Check if the maximum number of clients has been reached
        connectedClientsMutex.lock();
        if (connectedClients.size() >= MAX_CLIENTS) {
            std::cout << "Maximum number of clients reached. Rejecting connection." << std::endl;
            close(clientSocket);
            connectedClientsMutex.unlock();
            continue;
        }

        //Create a new client object
        Client* client = new Client(clientSocket);

        //Add the client to the list of connected clients
        connectedClients.push_back(client);
        connectedClientsMutex.unlock();

        //Create a new thread to handle the client
        std::thread clientThread(&Server::handleClient, this, client);
        clientThread.detach();
    }

    //Close the server socket
    close(serverSocket);
}

void Server::stop()
{
    isRunning = false;

    //Send "Thank you" message to all connected clients
    std::string thankYouMessage = "Thank you\n";
    connectedClientsMutex.lock();
    for (Client* client : connectedClients)
    {
        send(client->getSocket(), thankYouMessage.c_str(), thankYouMessage.length(), 0);
        close(client->getSocket()); // Close the client socket
    }
    connectedClientsMutex.unlock();

    //Clear the list of connected clients
    connectedClients.clear();

    //Exit the application
    exit(0);
}

void Server::handleClient(Client* client)
{
    int clientSocket = client->getSocket();

    // Send a welcome message to the client
    std::string welcomeMessage = "Welcome to the server!\n";
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.length(), 0);

    //Generate and send a unique ID to the client every second
    std::string uniqueId;
    std::atomic<bool> isClientConnected(true);

    while (isClientConnected)
    {
        //Generate a new unique ID
        uniqueId = generateUniqueID();

        //Send the unique ID to the client
        send(clientSocket, uniqueId.c_str(), uniqueId.length(), 0);
        send(clientSocket, "\n", 1, 0);

        //Sleep for one second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //Check if the client is still connected
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead == 0 || (bytesRead == EXIT_COMMAND.length() && memcmp(buffer, EXIT_COMMAND.c_str(), EXIT_COMMAND.length()) == 0)) {
            isClientConnected = false;
        }
    }

    //Remove the client from the list of connected clients
    connectedClientsMutex.lock();
    connectedClients.erase(std::remove(connectedClients.begin(), connectedClients.end(), client),
                           connectedClients.end());
    connectedClientsMutex.unlock();

    //Close the client socket
    close(clientSocket);

    //Broadcast the updated client count
    broadcastClientCount();

    //Delete the client object
    delete client;
}

void Server::broadcastClientCount()
{
    std::string clientCount = "Number of clients connected: " + std::to_string(connectedClients.size()) + "\n";

    // Broadcast the client count to all connected clients
    connectedClientsMutex.lock();
    for (Client* client : connectedClients)
    {
        send(client->getSocket(), clientCount.c_str(), clientCount.length(), 0);
    }
    connectedClientsMutex.unlock();
}

std::string Server::generateUniqueID()
{
    static std::set<std::string> generatedIDs;
    std::string id;

    do
    {
        //Generate a new unique ID
        id = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());

        //Add the ID to the set of generated IDs
        generatedIDs.insert(id);
    } while (generatedIDs.size() > 6);

    return id;
}

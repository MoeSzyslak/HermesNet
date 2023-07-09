#include "server.h"
#include<iostream>
#include <signal.h>

Server server;

//Signal handler for Ctrl-C
void handleSignal(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "Ctrl-C received. Shutting down the server..." << std::endl;
        server.stop();
    }
}

int main(int argc, char* argv[])
{
    // Set up the signal handler
    signal(SIGINT, handleSignal);

    // Check if a port number is provided as a command-line argument
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    // Get the port number from the command-line argument
    int port = std::atoi(argv[1]);

    // Start the server
    server.setPort(port);


    // Start the server
    server.start();

    return 0;
}

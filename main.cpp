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

int main() 
{
    // Set up the signal handler
    signal(SIGINT, handleSignal);

    // Start the server
    server.start();

    return 0;
}

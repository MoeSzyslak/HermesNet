# HermesNet
HermesNet is a TCP server that handles multiple client connections
This is a simple TCP server implementation that can handle up to 6 TCP clients concurrently.
It generates and sends a unique 32-bit sequence number ID to each connected client every second.
The server also broadcasts the number of connected clients to all clients when a new client joins or leaves.
 The server can be gracefully stopped using the Ctrl-C signal, which sends a "Thank you" message to all connected clients before exiting.

## Compilation Steps

To compile the TCP server, follow these steps:

    Make sure you have CMake and a C++ compiler installed on your system.
    Clone the repository or download the source code files.
    Open a terminal or command prompt and navigate to the project directory.
    Create a build directory: mkdir build
    Navigate to the build directory: cd build
    Generate the build files using CMake: cmake ..
    Compile the project: make

## Running the Server

To run the TCP server, follow these steps:

    Make sure the server has been successfully compiled as explained in the compilation steps.
    From the build directory, run the server executable: ./server <port>
    The server will start and display a message indicating the port it is listening on.

## Testing the Server

To test the TCP server, you can use Telnet or any TCP client tool:

    Open a new terminal or command prompt.
    Connect to the server using Telnet or a TCP client tool: telnet <server-ip> <server-port>
    You should receive a welcome message from the server and start receiving unique IDs every second.
    Open multiple client connections to test the server's concurrent handling capability.
    To disconnect a client, simply close the Telnet or client tool connection.
    The server will automatically broadcast the updated number of connected clients to all remaining clients.

## Stopping the Server

To stop the TCP server gracefully, follow these steps:

    Press Ctrl-C in the terminal or command prompt where the server is running.
    The server will send a "Thank you" message to all connected clients.
    The server will close all client connections and release the listening port.
    The server application will exit.

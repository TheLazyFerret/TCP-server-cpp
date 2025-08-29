# tcp-wrapper-cpp
C++ wrapper to abstract the low-level TCP socket API.

This repository contains two independent libraries with similar APIs
- `tcp-server` -> Object-oriented abstraction of a TCP server.
- `tcp-client` -> Object-oriented abstraction of a TCP client.

**Note:** Only compatible with Unix-like systems.

## Usage
### Namespaces
- `tcp_server`: Contains server-side classes.
- `tcp_client`: Contains client-side classes.
- `tcp_exceptions`: Contains shared exception classes used by both libraries.

### Server (`tcp_server`)
This library provides two main classes: `TCPServer` and `TCPConnection`.

### TCPServer
Responsible for setting up the server socket and accepting incoming connections.
- `TCPServer::TCPServer(port, address)` -> Constructor, set the parameters but not initialize the server.
- `TCPServer::Initialize(backlog)` -> Set the socket file descriptor, binds it, and set to passive the socket.
- `TCPServer::Accept()` -> Returns an instance of `TCPConnection`. As `accept()`, it is a blocking call. Requires be initialized.
- `TCPServer::Kill()` -> Close the socket file descriptor. If it is not called before it object goes out of scope, is called by destructor. Requires be initialized.

### TCPConnection
Represents a single connection with a client. It is initialized automatically when `TCPServer::Accept()`
- `TCPConnection::Send(src*, len, flags)` -> Sends a message to the client. src can be a pointer to any type.
- `TCPConnection::Recv(dst*, len, flags)` -> Receives a message from the client. dst can be a pointer to any type.
- `TCPConnection::Kill()` -> Closes the connection. Automatically called by the destructor if not done manually.

### Client (`tcp_client`)
This library provides one single class: `TCPClient`.
### `TCPClient` 
Represents a single TCP client connected to a server connection.
- `TCPClient::TCPClient(port, address)` -> Constructor, sets the parameters but not initialize the client. 
- `TCPClient::Connect()`->  Initializes the socket and attempts to connect to the server.
- `TCPClient::Send(src*, len, flags)` -> Sends a message to the server connection.
- `TCPClient::Recv(dst*, len, flags)` -> Receives a message from the server connection.
- `TCPClient::Kill()` -> Closes the connection. Automatically called by the destructor if not done manually.

## Compilation
Not needed!  
Both libraries don't need individual compilation

## Example
- A very simple example of a server can be found in [server.cpp](example/server.cpp)
- A example of client can be found in [client.cpp](example/client.cpp)

## License
[MIT](LICENSE)
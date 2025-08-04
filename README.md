# tcp-wrapper-cpp
library whose objective is to abstract as much as possible from the C methods.

Only for unix systems.

## ToDo
- [x] TCPServer
- [ ] TCPClient

## Usage
The library is divided into two classes: `TCPServer` and `TCPConnection.`

`TCPConnection` cannot be instantiated from `main`, only from the `TCPServer::Connect()` method.

The library is inside the namespace `tcp_server`, while the exceptions are inside `tcp_exception`. It is only required to `#include tcp_server.hpp`, due already includes `tcp_exception.hpp`.

### `TCPServer`
The main class of the library, responsible for accepting incoming connections.
- `TCPServer::TCPServer(port, address)` -> Constructor, set the parameters but NOT initialize the server.
- `TCPServer::Initialize(backlog)` -> Set the socket file descriptor, binds it, and set to passive the socket.
- `TCPServer::Accept()` -> Returns an object of type `TCPConnection`. As accept(), it is a blocking call. Requires be initialized.
- `TCPServer::Kill()` -> close the socket file descriptor. If it is not called before it object goes out of scope, is called by destructor. Requires be initialized.

### `TCPConnection`
Class representing each connection with a client. It is automatically initialized when it is instantiated.
- `TCPConnection::Send(src*, len, flags)` -> Transmit a message to the client. src can be a pointer of any type.
- `TCPConnection::Recv(dst*, len, flags)` -> Recover a message sent by the client. src can be a pointer of any type.
- `TCPConnection::Kill()` -> close the socket file descriptor. If it is not called before it object goes out of scope, is called by destructor.

## Compilation
Can be compiled using `cmake` and `make`, generating a static library `TCPServer` (libTCPServer.a)

## Example
A very simple example can be found in [server.cpp](example/server.cpp)

## License
[MIT](LICENSE)

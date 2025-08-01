/**
 * @file TCP-server.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief implementation file of the TCP server.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

#include "TCP-server.hpp"

#ifdef DEBUG
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[!] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

/// @brief Set the max ammount of connection the socket will accept.
constexpr int KMaxConnections = 2;

/// @brief Constructor of the class TCPServer
/// @param port 
/// @param address 
/// @param buffer_size
TCPServer::TCPServer(const unsigned short port, const std::string& address, const size_t buffer_size) : buffer_(nullptr), buffer_size_(0) {
  // Initialize socket
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw(InitializeSocketException());
  }
  DEBUG_PRINT("Socket file descriptor initialized in: " << socket_fd_);

  // Initialize address
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr = ConvertAddrBinary(address);
  addr_.sin_port = htons(port);

  // Initialize buffer
  buffer_ = new unsigned char[buffer_size];
  buffer_size_ = buffer_size;
  DEBUG_PRINT("Buffer allocated with size: " << buffer_size_);
}

/// @brief Destructor of the class TCPServer
TCPServer::~TCPServer() {
  Kill();
  delete[] buffer_;
  buffer_ = 0;
}

/// @brief Start the server, Binding and setting it in passive mode
void TCPServer::Initialize() {
  // Bind the socket file descriptor to the address and port.
  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&addr_);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(addr_));
  if (bind(socket_fd_, aux_pointer, aux_size) < 0) {
    throw(BindingException());
  }
  DEBUG_PRINT("Socket binded to address: " << addr_.sin_addr.s_addr);
  DEBUG_PRINT("Socket binded to port: " << ntohs(addr_.sin_port));

  // Set the socket to passive mode.
  if (listen(socket_fd_, KMaxConnections) < 0) {
    throw(ListeningException());
  }
  DEBUG_PRINT("Socket set to passive mode with: " << KMaxConnections << " connections");
}


/// @brief Calls the subrutine "Accept()" and waiting untils a client request.
/// @return The addr of the client.
sockaddr_in TCPServer::Accept() {
  sockaddr_in client;
  memset(&client, 0, sizeof(client));
  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&client);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(client));

  if (accept(socket_fd_, aux_pointer, &aux_size) < 0) {
    std::cout << "errno: " << errno << std::endl;
    throw(AcceptException()); 
  }

  DEBUG_PRINT("Accepted connection request from client with address: " << ntohl(client.sin_addr.s_addr));
  return client;
 }

/// @brief Kill the server, closing the socket. Keeps the buffer
void TCPServer::Kill() noexcept {
  if (socket_fd_ >= 0) {
    close(socket_fd_);
    socket_fd_ = -1;
  }
}

/// @brief Converts a IPv4 number and dots notation into binary
/// @param address 
/// @return 
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) < 0) {
    throw(ConvertBinaryAddrException());
  }

  DEBUG_PRINT("Address converted from: " << address << " to: " << addr.s_addr);
  return addr;
}
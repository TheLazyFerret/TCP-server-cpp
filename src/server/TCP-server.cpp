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

/// @brief Constructor of the class TCPServer
/// @param port 
/// @param address 
/// @param buffer_size
TCPServer::TCPServer(const unsigned short port, const std::string& address, const size_t buffer_size) : buffer_(nullptr), buffer_size_(0) {
  InitializeSocket();
  InitializeAddress(port, ConvertAddrBinary(address));

  buffer_ = new unsigned char[buffer_size];
  buffer_size_ = buffer_size;
}

/// @brief Destructor of the class TCPServer
TCPServer::~TCPServer() {
  Kill();
  delete[] buffer_;
  buffer_ = 0;
}

/// @brief Binds the socket to an ip and a address.
void TCPServer::Bind() {
  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&addr_);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(addr_));

  if (bind(socket_fd_, aux_pointer, aux_size) < 0) {
    throw(BindingException());
  }
}

void TCPServer::Listen() {
  if (listen(socket_fd_, 5) < 0) {
    throw(ListeningException());
  }
}

void TCPServer::Accept() {
  sockaddr_in client;
  memset(&client, 0, sizeof(client));

  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&client);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(client));

  std::cout << "accepting connections..." << std::endl;

  if (accept(socket_fd_, aux_pointer, &aux_size) < 0) {
    throw(AcceptException()); 
  }
 
  std::cout << "connection accepted from: " << client.sin_addr.s_addr << std::endl;
}

/// @brief Kill the server, closing the socket. Keeps the buffer
void TCPServer::Kill() noexcept {
  if (socket_fd_ >= 0) {
    close(socket_fd_);
    socket_fd_ = -1;
  }
}

/// @brief Initialize the file descriptor for the socket
void TCPServer::InitializeSocket() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw(InitializeSocketException());
  }
}

/// @brief Initialize the address of the server
/// @param port 
/// @param addr 
void TCPServer::InitializeAddress(const unsigned short port, const in_addr& addr) noexcept {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = INADDR_ANY;
  addr_.sin_port = htons(port);
}

/// @brief Converts a IPv4 number and dots notation into binary
/// @param address 
/// @return 
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) < 0) {
    throw(ConvertBinaryAddrException());
  }
  return addr;
}
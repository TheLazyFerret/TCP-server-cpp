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
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[+] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

constexpr int KDefault_Backlog = 5;

/// @brief Constructor of TCPServer.
/// @param port 
/// @param address 
TCPServer::TCPServer(const unsigned short port, const std::string& address) {
  // Initialize attributes.
  initialized_ = false;
  socket_fd_ = -1;
  memset(&socket_addr_, 0, sizeof(socket_addr_));

  // Initialize address.
  socket_addr_.sin_family = AF_INET;
  socket_addr_.sin_port = htons(port);
  socket_addr_.sin_addr = ConvertAddrBinary(address);
}

/// @brief Initialize the server.
///   Create the socket file descriptor, bind to the address and set to passive mode.
///   If it is already initialized, doesn't do anything.
/// @param backlog 
void TCPServer::Initialize(int backlog) {
  if (initialized_) return;
  // Initialize socket file descriptor.
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket file descriptor created with code: " << socket_fd_);

  // Bind the socket file descriptor to an address.
  const sockaddr* aux_addr_pointer = reinterpret_cast<sockaddr*>(&socket_addr_);
  const socklen_t aux_addr_len = static_cast<socklen_t>(sizeof(socket_addr_));
  if (bind(socket_fd_, aux_addr_pointer, aux_addr_len) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket bound to the port: " << ntohs(socket_addr_.sin_port));

  // Set the socket to passive mode.
  if (backlog <= 0) backlog = KDefault_Backlog;
  if (listen(socket_fd_, backlog) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket set to passive mode");
  initialized_ = true;
}

/// @brief Convert a string address into a binary (in network order) address.
/// @param address 
/// @return The address in binary.
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) == 0) {
    throw ConvertBinaryException();
  }
  DEBUG_PRINT("Address converted from: " << address << " to: " << ntohl(addr.s_addr));
  return addr;
}

/// @brief Close the socket. 
///   If it is already closed, doesn't do anything.
void TCPServer::Kill() {
  if (!initialized_) return;
  else if (close(socket_fd_) < 0) {
    throw ErrnoException(errno);
  }
  socket_fd_ = -1;
  initialized_ = false;
}
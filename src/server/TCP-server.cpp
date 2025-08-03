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

/// @brief Constructor of TCPServer
/// @param port 
/// @param address 
TCPServer::TCPServer(const unsigned short port, const std::string& address) {
  initialized_ = false;
  // Initialize socket file descriptor.
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket file descriptor created with code: " << socket_fd_);
  
  // Initialize address.
  memset(&socket_addr_, 0, sizeof(socket_addr_));
  socket_addr_.sin_family = AF_INET;
  socket_addr_.sin_port = htons(port);
  socket_addr_.sin_addr = ConvertAddrBinary(address);
}

/// @brief Convert a string address into a binary (in network order) address
/// @param address 
/// @return the address in binary
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Address converted from: " << address << " to: " << ntohl(addr.s_addr));
  return addr;
}

void TCPServer::Initialize(const int backlog) {
  // Bind the socket file descriptor to an address.
  const sockaddr* aux_addr_pointer = reinterpret_cast<sockaddr*>(&socket_addr_);
  const socklen_t aux_addr_len = static_cast<socklen_t>(sizeof(socket_addr_));
  if (bind(socket_fd_, aux_addr_pointer, aux_addr_len) < 0) {
    throw ErrnoException(errno);
  }
  // Set the socket to passive mode.
  
}
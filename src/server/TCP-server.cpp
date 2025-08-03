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
void TCPServer::Initialize(const int backlog) {
  if (initialized_) return;
  // Initialize the socket file descriptor.
  InitializeSocket();
  // Bind the socket to the address and port.
  BindSocket();
  // Set the socket to passive mode.
  SetPassive(backlog);
  
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
  const int temp_socket_fd = socket_fd_;
  socket_fd_ = -1;
  initialized_ = false;
  if (close(temp_socket_fd) < 0) {
    throw ErrnoException(errno);
  }
}

/// @brief Initialize the socket file descriptor.
void TCPServer::InitializeSocket() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket file descriptor created with code: " << socket_fd_);
}

/// @brief Bind the socket file descriptor to the address.
void TCPServer::BindSocket() {
  const sockaddr* aux_addr_pointer = reinterpret_cast<sockaddr*>(&socket_addr_);
  const socklen_t aux_addr_len = static_cast<socklen_t>(sizeof(socket_addr_));
  if (bind(socket_fd_, aux_addr_pointer, aux_addr_len) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket bound to the port: " << ntohs(socket_addr_.sin_port));
}

/// @brief Set the socket file descriptor to passive.
/// @param backlog
void TCPServer::SetPassive(const int backlog) {
  const int effect_backlog = (backlog <= 0) ? KDefault_Backlog : backlog;
  if (listen(socket_fd_, effect_backlog) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket set to passive mode with backlog size: " << effect_backlog);
}
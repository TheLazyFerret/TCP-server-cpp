/**
 * @file TCP_server_connection.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Implementation file for the TCPConnection.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

#include "TCP_server.hpp"

#ifdef DEBUG
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[CONNECTION] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

/// @brief Private constructor of TCPConnection
///   It is private due not intended to being called directly,
///   but from a TCPServer::Accept()
/// @param socket_fd 
/// @param addr 
/// @param buffer_size 
TCPConnection::TCPConnection(const int socket_fd, const sockaddr_in& addr) 
: socket_fd_(socket_fd), addr_(addr), initialized_(true) {}

/// @brief Destructor of the class TCPConnection.
TCPConnection::~TCPConnection() {
  try {
    Kill();
  } catch(const TCPServerException& e) {
    DEBUG_PRINT("Error calling Kill(): " << e.what());
  }
}

/// @brief Close the connection.
void TCPConnection::Kill() {
  if (!initialized_) return;
  const int temp_socket_fd = socket_fd_;
  socket_fd_ = -1;
  initialized_ = false;
  if (close(temp_socket_fd) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket closed");
}

/// @brief Send a len ammount of bytes from src.
/// @param src 
/// @param len
/// @return the number of bytes sent
size_t TCPConnection::Send(const void* src, const size_t len, const int flags) const {
  if (src == nullptr) {
    throw InvalidPointer();
  }
  const ssize_t result = send(socket_fd_, src, len, flags);
  if (result < 0) {
    throw ErrnoException(errno);
  }
  return static_cast<size_t>(result);
}
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
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[CLIENT] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

/// @brief Private constructor of TCPConnection
///   It is private due not intended to being called directly,
///   but from a TCPServer::Accept()
/// @param socket_fd 
/// @param addr 
/// @param buffer_size 
TCPConnection::TCPConnection(const int socket_fd, const sockaddr_in& addr, const size_t buffer_size) 
  : socket_fd_(socket_fd), addr_(addr), initialized_(true) {
  buffer_size_ = (buffer_size == 0) ? Kdefault_buffer_ : buffer_size;
  buffer_ = new unsigned char[buffer_size_];
}

/// @brief Move assign.
/// @param aux 
TCPConnection::TCPConnection(TCPConnection&& aux) {
  Move(*this, aux);
}

/// @brief Move operator.
/// @param aux 
/// @return a reference of the caller instance.
TCPConnection& TCPConnection::operator=(TCPConnection&& aux) {
  Move(*this, aux);
  return *this;
}

/// @brief Move the resources of source to s. 
///   Also, first free the memory allocated by s.
/// @param s 
/// @param source 
void TCPConnection::Move(TCPConnection& s, TCPConnection& source) {
  if (&s == &source) {
    return;
  }
  // Clean old state
  if (s.buffer_ != nullptr) delete[] s.buffer_;
  if (s.initialized_) s.Kill();
  // Move the data
  s.socket_fd_ = source.socket_fd_;
  s.addr_ = source.addr_;
  s.buffer_ = source.buffer_;
  s.buffer_size_ = source.buffer_size_;
  s.initialized_ = source.initialized_;
  // Erase old instance
  source.socket_fd_ = -1;
  memset(&source.addr_, 0, sizeof(source.addr_));
  source.buffer_ = nullptr;
  source.buffer_size_ = 0;
  source.initialized_ = false;
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

/// @brief Destructor of the class TCPConnection.
TCPConnection::~TCPConnection() {
  try {
    Kill();
  } catch(const TCPServerException& e) {
    DEBUG_PRINT("Error calling Kill(): " << e.what());
  }
  if (buffer_ != nullptr) delete[] buffer_;
  buffer_ = nullptr;
  buffer_size_ = 0;
}
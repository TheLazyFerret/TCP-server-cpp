/**
 * @file tcp_server.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Implementation file for the TCPServer.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

#include "tcp_server.hpp"

#ifdef DEBUG
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[SERVER] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

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

/// @brief Destructor of TCPServer.
TCPServer::~TCPServer() {
  if (!initialized_) {
    return;
  }
  try {
    Kill();
  } catch(const TCPServerException& e) {
    DEBUG_PRINT("Error calling Kill(): " << e.what());
  } 
}

/// @brief Close the socket. 
///   If it is already closed, doesn't do anything.
///   Can throw an exception if the Server is not initialized.
void TCPServer::Kill() {
  if (!initialized_) {
    throw NotInitialized();
  }
  const int temp_socket_fd = socket_fd_;
  socket_fd_ = -1;
  initialized_ = false;
  if (close(temp_socket_fd) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket closed");
}

/// @brief Initialize the server.
///   Create the socket file descriptor, bind to the address and set to passive mode.
///   If it is already initialized, doesn't do anything.
/// @param backlog 
void TCPServer::Initialize(const int backlog) {
  if (initialized_) {
    DEBUG_PRINT("Already initialized");
    return;
  }
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
  memset(&addr, 0, sizeof(addr));
  const int result = inet_pton(AF_INET, address.c_str(), &addr);
  if (result == 0) {
    throw ConvertBinaryException(address);
  }
  else if (result < 0) {
    throw ErrnoException(errno);
  }
  //DEBUG_PRINT("Address converted from: " << address << " to: " << ntohl(addr.s_addr));
  return addr;
}

/// @brief Convert a Binary address to a string format.
/// @param address 
/// @return The address as a string.
std::string TCPServer::ConvertAddrString(const in_addr& address) {
  char str[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &address, str, INET_ADDRSTRLEN) == nullptr) {
    throw ErrnoException(errno);
  }
  const std::string result(str);
  //DEBUG_PRINT("Address converted from: " << ntohl(address.s_addr) << " to: " << result);
  return result;
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
  const int effective_backlog = (backlog <= 0) ? KDefaultBacklog : backlog;
  if (listen(socket_fd_, effective_backlog) < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Socket set to passive mode with backlog size: " << effective_backlog);
}

/// @brief Call accept() and instance a new object TCPConnection
///   This is the only method for instance this class.
///   Can throw an exception if the server is not initialized.
/// @param buffer_size 
/// @return a new instance of TCPConnection.
TCPConnection TCPServer::Accept() const {
  if (!initialized_) {
    throw NotInitialized();
  }
  // Main variables
  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_addr_len = static_cast<socklen_t>(sizeof(client_addr));
  // Auxiliar pointers
  sockaddr* client_addr_aux = reinterpret_cast<sockaddr*>(&client_addr);
  socklen_t* client_addr_len_aux = &client_addr_len;
  // Listen call
  const int client_socket = accept(socket_fd_, client_addr_aux, client_addr_len_aux);
  if (client_socket < 0) {
    throw ErrnoException(errno);
  }
  DEBUG_PRINT("Accepted conection from: " << ConvertAddrString(client_addr.sin_addr));
  return TCPConnection(client_socket, client_addr);
}

#undef DEBUG_PRINT
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
  if (!initialized_) {
    return;
  }
  try {
    Kill();
  } catch(const TCPServerException& e) {
    DEBUG_PRINT("Error calling Kill(): " << e.what());
  }
}

/// @brief Close the connection.
void TCPConnection::Kill() {
  if (!initialized_) {
    throw NotInitialized();
  }
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
/// @return The number of bytes sent.
size_t TCPConnection::Send(const void* src, const size_t len, const int flags) const {
  if (!initialized_) {
    throw NotInitialized();
  }
  if (src == nullptr) {
    throw InvalidPointer();
  }
  const ssize_t result = send(socket_fd_, src, len, flags);
  if (result < 0) {
    throw ErrnoException(errno);
  }
  return static_cast<size_t>(result);
}

/// @brief Receive message from the socket fd and save it in the buffer src.
/// @param src 
/// @param len 
/// @param flags 
/// @return The number of bytes received.
size_t TCPConnection::Recv(void* dst, const size_t len, const int flags) const {
  if (!initialized_) {
    throw NotInitialized();
  }
  if (dst == nullptr) {
    throw InvalidPointer();
  }
  const ssize_t result = recv(socket_fd_, dst, len, flags);
  if (result < 0) {
    throw ErrnoException(errno);
  }
  return static_cast<size_t>(result);
}
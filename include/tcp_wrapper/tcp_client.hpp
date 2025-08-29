/**
 * @file tcp_client.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Library wrapping all the quirks of a client socket programming. Uses moderm c++ features.
 */

#ifndef TCP_WRAPPER_CLIENT_HPP
#define TCP_WRAPPER_CLIENT_HPP

#include "tcp_wrapper/tcp_exception.hpp"
#include "tcp_wrapper/tcp_shared.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

namespace tcp_client {

class TCPClient {
  public:
    // Special methods
    TCPClient() = delete;
    TCPClient(const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    TCPClient(TCPClient&&) = delete;
    TCPClient& operator=(TCPClient&&) = delete;

    TCPClient(const unsigned short port, const std::string& address);
    ~TCPClient();
    // Normal methods
    void Connect();
    void Kill();

    inline bool IsInitialized() const noexcept {return initialized_;}

    size_t Send(const void* src, const size_t len, const int flags = 0) const;
    size_t Recv(void* dst, const size_t len, const int flags = 0) const;

  private:
    // Private methods
    void InitializeSocket();

    // Attributes
    int socket_fd_;
    sockaddr_in socket_addr_;
    bool initialized_;
};

}

#ifdef DEBUG
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[CLIENT] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

/// @brief Constructor of TCPClient.
/// @param port 
/// @param address 
inline tcp_client::TCPClient::TCPClient(const unsigned short port, const std::string& address)
: socket_fd_(-1), initialized_(false) {
  // Initialize address
  memset(&socket_addr_, 0, sizeof(socket_addr_));
  socket_addr_.sin_family = AF_INET;
  socket_addr_.sin_port = htons(port);
  socket_addr_.sin_addr = tcp_shared::ConvertAddrBinary(address);
}

/// @brief Destructor of TCPClient.
inline tcp_client::TCPClient::~TCPClient() {
  if (!initialized_) {
    return;
  }
  try {Kill();}
  catch (const tcp_exception::TCPException& e) {DEBUG_PRINT("Error calling Kill(): " << e.what());}
}

/// @brief Connect the socket_fd_ to a server.
inline void tcp_client::TCPClient::Connect() {
  InitializeSocket();
  const sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&socket_addr_);
  const socklen_t aux_len = static_cast<socklen_t>(sizeof(socket_addr_));
  if (connect(socket_fd_, aux_pointer, aux_len) < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
  initialized_ = true;
  DEBUG_PRINT("Client succesfully connected to the server: " 
    << tcp_shared::ConvertAddrSring(socket_addr_.sin_addr));
}

/// @brief Initialize the socket file descriptor.
inline void tcp_client::TCPClient::InitializeSocket() {
  socket_fd_ = tcp_shared::InitializeSocket();
  DEBUG_PRINT("Socket file descriptor created with code: " << socket_fd_);
}

/// @brief Close the connection if it is open.
inline void tcp_client::TCPClient::Kill() {
  if (!initialized_) {
    throw tcp_exception::NotInitialized();
  }
  tcp_shared::KillSocketfd(socket_fd_);
  initialized_ = false;
  DEBUG_PRINT("Connection closed");
}

/// @brief Send a len ammount of bytes from src.
/// @param src 
/// @param len 
/// @param flags 
/// @return The number of bytes sent.
inline size_t tcp_client::TCPClient::Send(const void* src, const size_t len, const int flags) const {
  if (!initialized_) {
    throw tcp_exception::NotInitialized();
  }
  const size_t result = tcp_shared::Send(src, len, flags, socket_fd_);
  DEBUG_PRINT("Sent: " << result << " bytes");
  return result;
}

/// @brief Receive message from the socket fd and save it in the buffer src.
/// @param src 
/// @param len 
/// @param flags 
/// @return The number of bytes received.
inline size_t tcp_client::TCPClient::Recv(void* dst, const size_t len, const int flags) const {
  if (!initialized_) {
    throw tcp_exception::NotInitialized();
  }
  const size_t result = tcp_shared::Recv(dst, len, flags, socket_fd_);
  DEBUG_PRINT("Received: " << result << " bytes");
  return result;
}

#endif
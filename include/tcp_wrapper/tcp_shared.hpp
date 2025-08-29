/**
 * @file tcp_shared.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief common functions used by both libraries.
 */

#ifndef TCP_SHARED_HPP
#define TCP_SHARED_HPP

#include "tcp_wrapper/tcp_exception.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

namespace tcp_internal {

inline in_addr ConvertAddrBinary(const std::string& address);
inline std::string ConvertAddrSring(const in_addr& address);

inline int InitializeSocket();
inline void KillSocketfd(int& socket_fd);

inline size_t Send(const void* src, const size_t len, const int flags, const int socket_fd);
inline size_t Recv(void* dst, const size_t len, const int flags, const int socket_fd);

}

/// @brief Convert a string IPv4 addr to binary (in ip format).
/// @param address 
/// @return the address in binary as a in_addr struct.
in_addr tcp_internal::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  memset(&addr, 0, sizeof(addr));
  const int result = inet_pton(AF_INET, address.c_str(), &addr);
  if (result == 0) {
    throw tcp_exception::ConvertBinaryException(address);
  }
  else if (result < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
  return addr;
}

/// @brief Convert a binary address to a human redeable string address.
/// @param address 
/// @return the address as a std::string.
std::string tcp_internal::ConvertAddrSring(const in_addr& address) {
  char str[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &address, str, INET_ADDRSTRLEN) == nullptr) {
    throw tcp_exception::ErrnoException(errno);
  }
  const std::string result(str);
  return result;
}

/// @brief Initialize the socket file descriptor.
/// @return The file descriptor.
int tcp_internal::InitializeSocket() {
  const int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
  return fd;
}

/// @brief Close a socket file descriptor.
/// @param socket_fd 
void tcp_internal::KillSocketfd(int& socket_fd) {
  if (socket_fd < 0) {
    return;
  }
  const int temp_socket_fd = socket_fd;
  socket_fd = -1;
  if (close(temp_socket_fd) < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
}

/// @brief Send len bytes from src to the socket.
/// @param src 
/// @param len 
/// @param flags 
/// @param socket_fd 
/// @return The number of bytes sent.
size_t tcp_internal::Send(const void* src, const size_t len, const int flags, const int socket_fd) {
  if (src == nullptr) {
    throw tcp_exception::InvalidPointer();
  }
  const ssize_t result = send(socket_fd, src, len, flags);
  if (result < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
  return static_cast<size_t>(result);
}

/// @brief Receive message from the socket fd and save it in the buffer src.
/// @param src 
/// @param len 
/// @param flags 
/// @param socket_fd
/// @return The number of bytes received.
size_t tcp_internal::Recv(void* dst, const size_t len, const int flags, const int socket_fd) {
  if (dst == nullptr) {
    throw tcp_exception::InvalidPointer();
  }
  const ssize_t result = recv(socket_fd, dst, len, flags);
  if (result < 0) {
    throw tcp_exception::ErrnoException(errno);
  }
  return static_cast<size_t>(result);
}

#endif
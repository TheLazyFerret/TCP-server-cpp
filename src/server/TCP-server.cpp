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
  #define TCP_DEBUG_PRINT(MESSAGE) std::cerr << "[!] " <<  MESSAGE << std::endl
#else
  #define TCP_DEBUG_PRINT(MESSAGE)
#endif

/// @brief Set the max amount of connection the socket will accept.
constexpr int KMaxConnections = 2;
constexpr size_t KDefaultBufferSize = 100;

/// @brief Constructor of the class TCPServer
/// @param port 
/// @param address 
/// @param buffer_size
TCPServer::TCPServer(const unsigned short port, const std::string& address, const size_t buffer_size) 
: send_buffer_(nullptr), recv_buffer_(nullptr), buffer_size_(0) {
  // Initialize socket
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw(InitializeSocketException(errno));
  }
  TCP_DEBUG_PRINT("Socket file descriptor initialized in: " << socket_fd_);

  // Initialize address
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr = ConvertAddrBinary(address);
  addr_.sin_port = htons(port);

  // Initialize buffer
  const size_t size = (buffer_size == 0) ? KDefaultBufferSize : buffer_size;
  send_buffer_ = new unsigned char[size];
  recv_buffer_ = new unsigned char[size];
  buffer_size_ = size;
  TCP_DEBUG_PRINT("Buffer allocated with size: " << size);
}

/// @brief Destructor of the class TCPServer
TCPServer::~TCPServer() {
  Kill();
  delete[] send_buffer_;
  send_buffer_ = nullptr;
  delete[] recv_buffer_;
  recv_buffer_ = nullptr;
  buffer_size_ = 0;
}

/// @brief Start the server, Binding and setting it in passive mode
void TCPServer::Initialize() {
  // Bind the socket file descriptor to the address and port.
  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&addr_);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(addr_));
  if (bind(socket_fd_, aux_pointer, aux_size) < 0) {
    throw(BindingException(errno));
  }
  TCP_DEBUG_PRINT("Socket bound to address: " << addr_.sin_addr.s_addr);
  TCP_DEBUG_PRINT("Socket bound to port: " << ntohs(addr_.sin_port));

  // Set the socket to passive mode.
  if (listen(socket_fd_, KMaxConnections) < 0) {
    throw(ListeningException(errno));
  }
  TCP_DEBUG_PRINT("Socket set to passive mode with: " << KMaxConnections << " connections");
}


/// @brief Calls the subrutine "Accept()" and waiting until a client request.
/// @return The addr of the client.
sockaddr_in TCPServer::Accept() {
  sockaddr_in client;
  memset(&client, 0, sizeof(client));
  sockaddr* aux_pointer = reinterpret_cast<sockaddr*>(&client);
  socklen_t aux_size = static_cast<socklen_t>(sizeof(client));

  if (accept(socket_fd_, aux_pointer, &aux_size) < 0) {
    throw(AcceptException(errno)); 
  }

  TCP_DEBUG_PRINT("Accepted connection request from client with address: " << ntohl(client.sin_addr.s_addr));
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
/// @return the IPv4 in binary.
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) < 0) {
    throw(ConvertBinaryAddrException(errno));
  }
  TCP_DEBUG_PRINT("Address converted from: " << address << " to: " << addr.s_addr)
  return addr;
}

/// @brief Set the send buffer.
///   Choose the smallest between buffer_size_ and n_bytes. if n_bytes == 0, uses buffer_size_
/// @param ext 
/// @param n_bytes 
/// @return 
size_t TCPServer::SetBuffer(const void* ext, const size_t n_bytes) noexcept {
  if (ext == nullptr) {
    return 0;
  }
  const size_t bytes_to_copy = (n_bytes == 0) ? buffer_size_ : std::min(n_bytes, buffer_size_);
  TCP_DEBUG_PRINT("Number of bytes to copy (SetBuffer): " << bytes_to_copy)
  
  std::memcpy(send_buffer_, ext, bytes_to_copy);
  return bytes_to_copy;
}

/// @brief Safe send() implementation.
/// @param n_bytes 
/// @param flags 
/// @return the number of bytes sent.
size_t TCPServer::Send(const size_t n_bytes, const int flags) {
  const size_t bytes_to_send = (n_bytes == 0) ? buffer_size_ : std::min(buffer_size_, n_bytes);
  TCP_DEBUG_PRINT("Number of bytes to send: " << bytes_to_send)
  ssize_t result = send(socket_fd_, send_buffer_, bytes_to_send, flags);
  if (result < 0) {
    throw(SendException(errno));
  }
  return static_cast<size_t>(result);
}

/// @brief Safe recv() implementation.
/// @param n_bytes 
/// @param flags 
/// @return the number of bytes recovered from the socket.
size_t TCPServer::Recv(const size_t n_bytes, const int flags) {
  const size_t bytes_to_recv = (n_bytes == 0) ? buffer_size_ : std::min(buffer_size_, n_bytes);
  TCP_DEBUG_PRINT("Number of bytes to recover: " << bytes_to_recv)
  ssize_t result = recv(socket_fd_, recv_buffer_, bytes_to_recv, flags);
  if (result < 0) {
    throw(RecvException(errno));
  }
  return static_cast<size_t>(result);
}

/// @brief Get the get buffer.
/// @param dest 
/// @param n_bytes 
/// @return the number of bytes got.
size_t TCPServer::GetBuffer(void* dest, const size_t n_bytes) const noexcept {
  if (dest == nullptr) {
    return 0;
  }
  const size_t bytes_to_copy = (n_bytes == 0) ? buffer_size_ : std::min(buffer_size_, n_bytes);
  TCP_DEBUG_PRINT("Number of bytes to copy (GetBuffer): " << bytes_to_copy)
  
  std::memcpy(dest, recv_buffer_, bytes_to_copy);
  return bytes_to_copy;
}
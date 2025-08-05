/**
 * @file tcp_client.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief implementation file of TCPClient.
 */

#include "tcp_client.hpp"
#include "tcp_internal.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

#ifdef DEBUG
  #define DEBUG_PRINT(MESSAGE) std::cerr << "[CLIENT] " <<  MESSAGE << std::endl
#else
  #define DEBUG_PRINT(MESSAGE)
#endif

using namespace tcp_exception;
using namespace tcp_client;

/// @brief Constructor of TCPClient.
/// @param port 
/// @param address 
TCPClient::TCPClient(const unsigned short port, const std::string& address) 
: socket_fd_(-1), initialized_(false) {
  // Initialize address
  memset(&socket_addr_, 0, sizeof(socket_addr_));
  socket_addr_.sin_family = AF_INET;
  socket_addr_.sin_port = htons(port);
  socket_addr_.sin_addr = tcp_internal::ConvertAddrBinary(address);
}

void TCPClient::Connect() {

}

void TCPClient::InitializeSocket() {
  socket_fd_ = tcp_internal::InitializeSocket();
  DEBUG_PRINT("Socket file descriptor created with code: " << socket_fd_);
}
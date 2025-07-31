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

/// @brief Constructor of the class TCPServer
/// @param port 
/// @param address 
TCPServer::TCPServer(const unsigned short port, const std::string& address) {

}

/// @brief Initialize the file descriptor for the socket
void TCPServer::InitializeSocket() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw(InitializeSocketException());
  }
}

/// @brief Initialize the address of the server
/// @param port 
/// @param addr 
void TCPServer::InitializeAddress(const unsigned short port, const in_addr& addr) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr = addr;
  addr_.sin_port = htons(port);
}

/// @brief Converts a IPv4 number and dots notation into binary
/// @param address 
/// @return 
in_addr TCPServer::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  if (inet_aton(address.c_str(), &addr) < 0) {
    throw(ConvertBinaryAddrException());
  }
  return addr;
}
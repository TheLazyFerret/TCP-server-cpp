/**
 * @file client.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Example client program for debugging server.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>

// Reminder that I need to use htonl if using INADDR_LOOPBACK :(

int main() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cout << "error socket()" << std::endl;
    return 1;
  }
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5000);

  sockaddr* addr_pointer = reinterpret_cast<sockaddr*>(&addr);
  socklen_t addr_len = static_cast<socklen_t>(sizeof(addr));

  connect(socket_fd, addr_pointer, addr_len);

  close(socket_fd);
}
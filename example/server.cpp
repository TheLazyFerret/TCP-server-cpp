/**
 * @file server.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Main file example of TCPServer usage.
 */

#include "../include/tcp_wrapper/tcp_server.hpp"

#include <iostream>

int main() {
  tcp_server::TCPServer server(5000, "127.0.0.1");
  server.Initialize();
  tcp_server::TCPConnection connection = server.Accept();

  constexpr size_t buffer_size = 8 + 1;
  char buffer[buffer_size];

  connection.Recv(buffer, buffer_size);
  std::cout << "OUTPUT: " << std::string(buffer) << std::endl;

  server.Kill();
  connection.Kill();


  return 0;
}
/**
 * @file server.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Main file of server.
 */

#include <iostream>

#include "TCP-server.hpp"

#define PORT 5566

int main(void) {
  std::cout << "Creating server..." << std::endl;
  TCPServer server(PORT, "127.0.0.1", 100);

  std::cout << "Binding..." << std::endl;
  server.Bind();

  std::cout << "Listening..." << std::endl;
  server.Listen();

  std::cout << "Waiting for connection..." << std::endl;
  server.Accept();

  std::cout << "Shutting down..." << std::endl;
  server.Kill();
  return 0;
}
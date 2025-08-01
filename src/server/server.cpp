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
  TCPServer server(PORT, "127.0.0.1", 100);
  server.Bind();
  server.Listen();
  server.Accept();

  server.Kill();
  return 0;
}
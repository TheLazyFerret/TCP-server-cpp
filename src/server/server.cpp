/**
 * @file server.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Main file of server.
 */

#include <iostream>

#include "TCP_server.hpp"

int main(void) {
  TCPServer server(5000, "127.0.0.1");
  server.Initialize();
  TCPConnection client = server.Accept();

  server.Kill();
  client.Kill();

  return 0;
}
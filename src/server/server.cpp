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

int main(void) {
  TCPServer server(5000, "127.0.0.1", 100);
  server.Initialize();

  server.Kill();

  auto aux = server.Accept();
  std::cout << "cliente conectado desde: " << ntohl(aux.sin_addr.s_addr) << std::endl;

  return 0;
}
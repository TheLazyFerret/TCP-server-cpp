/**
 * @file client.cpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Main file example of TCPClient usage.
 */

// compilation example: g++ example/client.cpp -Iinclude -o client

#define DEBUG

#include "tcp_wrapper/tcp_client.hpp"

#include <iostream>
#include <string>

const std::string message{"HELLO :)"};

int main() {
  tcp_client::TCPClient client(5000, "127.0.0.1");
  client.Connect();

  const size_t buffer_size = message.length() + 1;

  client.Send(message.c_str(), buffer_size);

  client.Kill();
}
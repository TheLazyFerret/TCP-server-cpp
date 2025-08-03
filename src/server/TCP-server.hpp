/**
 * @file TCP-server.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Header file of the TCP server.
 */

#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <netinet/in.h>

#include <exception>
#include <string>
#include <cstring>

class TCPServer {
  public:
    // SPECIAL METHODS
    TCPServer() = delete;
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    TCPServer(const unsigned short port, const std::string& address);
    ~TCPServer();
 
    // NORMAL METHODS

  private:
    // PRIVATE METHODS

    // ATTRIBUTES

};

#endif
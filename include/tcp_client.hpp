/**
 * @file tcp_client.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Header file of TCPClient.
 */

#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "tcp_exception.hpp"
#include <netinet/in.h>
#include <string>

namespace tcp_client {

class TCPClient {
  public:
    // Special methods
    TCPClient() = delete;
    TCPClient(const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    TCPClient(TCPClient&&) = delete;
    TCPClient& operator=(TCPClient&&) = delete;

    TCPClient(const unsigned short port, const std::string& address);
    ~TCPClient();
    // Normal methods
    void Connect();
    void kill();

  private:
    // Private methods
    void InitializeSocket();

    // Attributes
    int socket_fd_;
    sockaddr_in socket_addr_;
    bool initialized_;
};

}

#endif
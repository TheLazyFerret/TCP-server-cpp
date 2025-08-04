/**
 * @file TCP_client.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Header file of TCPClient.
 */

#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <netinet/in.h>

#include <exception>
#include <string>
#include <cstring>

/// @brief Abstraction class for managing clients.
class TCPClient {
  public:

  private:
};

/// @brief Base class for representing all the exceptions of the class TCPClient.
class TCPClientException : public std::exception {
  public:
    TCPClientException(const std::string& message) : error_(message) {}
    const char* what() const noexcept override {return error_.c_str();}
  private:
    std::string error_;
};

#endif
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
    void Initialize(const int backlog = 0);
    void Kill();

  private:
    // PRIVATE METHODS
    in_addr ConvertAddrBinary(const std::string& address);
    void InitializeSocket();
    void BindSocket();
    void SetPassive(const int backlog);

    // ATTRIBUTES
    int socket_fd_;
    sockaddr_in socket_addr_;
    bool initialized_;

    static constexpr int KDefault_Backlog = 5;
};

/// @brief Base class for representing all the exceptions of the class TCPServer.
class TCPServerException : public std::exception {
  public:
    TCPServerException(const std::string& message) : error_(message) {}
    const char* what() const noexcept override {return error_.c_str();}
  private:
    std::string error_;
};

/// @brief Meta exception, that represents all error where errno is present.
class ErrnoException : public TCPServerException {
  public: 
    ErrnoException(const int error_code) : TCPServerException(std::string(std::strerror(error_code))) {}
};

/// @brief Exception called when tried to convert an invalid address.
class ConvertBinaryException : public TCPServerException {
  public:
    ConvertBinaryException(const std::string& address) : TCPServerException("Invalid address: " + address) {}
};

#endif
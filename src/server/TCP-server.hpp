/**
 * @file TCP-server.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Header file of the TCP server.
 */

#ifndef TCP_SERVER
#define TCP_SERVER

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <exception>
#include <string>
#include <cstring>

/// @brief Abstraction of a TCP server.
class TCPServer {
  public:
    // Special methods
    TCPServer() = delete;
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    TCPServer(const unsigned short port, const std::string& addr, const size_t buffer_size = 0);
    ~TCPServer();
    
    // Normal methods
    void Initialize();
    sockaddr_in Accept();
    size_t WrtBuffer(void* ext_buffer, size_t n_bytes = 0);
    size_t Send(const size_t n_bytes = 0, const int flags = 0);
    void Kill() noexcept;


  private:
    // Private methods
    static in_addr ConvertAddrBinary(const std::string& addr);

    // Attributes
    sockaddr_in addr_;
    int socket_fd_;
    unsigned char* buffer_;
    size_t buffer_size_;
};


/// @brief Exceptions of TCPServer
class TCPServerException : public std::exception {
  public:
    TCPServerException(const std::string& message) : message_(message) {}
    const char* what() const noexcept override {return message_.c_str();}
  private:
    std::string message_;
};

/// @brief Represents an error intializing the socket file descriptor.
class InitializeSocketException : public TCPServerException {
  public:
    InitializeSocketException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

/// @brief Represents an error converting from string to binary a IPv4 address.
class ConvertBinaryAddrException : public TCPServerException {
  public:
    ConvertBinaryAddrException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

/// @brief Represents an error binding the socket.
class BindingException : public TCPServerException {
  public:
    BindingException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

/// @brief Represents an error setting the socket in passive mode.
class ListeningException : public TCPServerException {
  public:
    ListeningException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

/// @brief Represents an error accepting a connection.
class AcceptException : public TCPServerException {
  public:
    AcceptException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

/// @brief Represents an error sending information.
class SendException : public TCPServerException {
  public:
    SendException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

class WrtException : public TCPServerException {
  public:
    WrtException(const int error_code) : TCPServerException(std::string(strerror(error_code))) {}
};

#endif

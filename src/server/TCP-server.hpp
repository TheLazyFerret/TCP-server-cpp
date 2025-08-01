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

#define MAX_CONNECTIONS 2

/// @brief Abstraction of a TCP server.
class TCPServer {
  public:
    // Special methods
    TCPServer() = delete;
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    TCPServer(const unsigned short port, const std::string& addr, const size_t buffer_size);
    ~TCPServer();
    
    // Normal methods
    void Bind();
    void Listen();
    sockaddr_in Accept();

    void Kill() noexcept;

  private:
    // Private methods
    void InitializeSocket();
    void InitializeAddress(const unsigned short port, const in_addr& addr) noexcept;
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
    InitializeSocketException() : TCPServerException("Error initializing the socket file descriptor.") {}
};

/// @brief Represents an error converting from string to binary a IPv4 address.
class ConvertBinaryAddrException : public TCPServerException {
  public:
    ConvertBinaryAddrException() : TCPServerException("Error converting the string address to binary.") {}
};

/// @brief Represents an error binding the socket.
class BindingException : public TCPServerException {
  public:
    BindingException() : TCPServerException("Error while binding the socket file descriptor.") {}
};

/// @brief Represents an error setting the socket in passive mode.
class ListeningException : public TCPServerException {
  public:
    ListeningException() : TCPServerException("Error setting the socket as passive.") {}
};

/// @brief Represents an error accepting a connection.
class AcceptException : public TCPServerException {
  public:
    AcceptException() : TCPServerException("Error accepting a connection.") {}
};

#endif

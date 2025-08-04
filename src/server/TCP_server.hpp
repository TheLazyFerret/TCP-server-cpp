/**
 * @file TCP_server.hpp
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

class TCPConnection;

/// @brief Abstraction class for managing server and create connections.
class TCPServer {
  public:
    // Special methods
    TCPServer() = delete;
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    TCPServer(const unsigned short port, const std::string& address);
    ~TCPServer();
 
    // Normal methods
    void Initialize(const int backlog = 0);

    void Kill();
    TCPConnection Accept() const;

    inline bool IsInitialized() const noexcept {return initialized_;}

  private:
    // Private methods
    static in_addr ConvertAddrBinary(const std::string& address);
    static std::string ConvertAddrString(const in_addr& address);
    void InitializeSocket();
    void BindSocket();
    void SetPassive(const int backlog);

    // Attributes
    int socket_fd_;
    sockaddr_in socket_addr_;
    bool initialized_;
    static constexpr int kDefaultBacklog = 5;
};

/// @brief Abstraction class for managing connections.
class TCPConnection {
  friend TCPServer;
  public:
    // Special methods
    TCPConnection() = delete;
    TCPConnection(const TCPConnection&) = delete;
    TCPConnection& operator=(const TCPConnection&) = delete;

    TCPConnection(TCPConnection&&) = default;
    TCPConnection& operator=(TCPConnection&&) = default;
    ~TCPConnection();

    // normal methods
    void Kill();
    inline bool IsInitialized() const noexcept {return initialized_;}
    size_t Send(const void* src, const size_t len, const int flags = 0) const;
    size_t Recv(void* dst, const size_t len, const int flags = 0) const;
  
  private:
    // Private methods
    TCPConnection(const int socket_fd, const sockaddr_in& addr);

    // Attributes
    int socket_fd_;
    sockaddr_in addr_;
    bool initialized_;
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

/// @brief Exception representing situations where trying to access a recurse while object not initialized correctly
class NotInitialized : public TCPServerException {
  public:
    NotInitialized() : TCPServerException("Trying to access a method while object not correctly initialized") {}
};

/// @brief Exception sent when a pointer is invalid.
///   Basically trying to avoid a seg fault.
class InvalidPointer : public TCPServerException {
  public:
    InvalidPointer() : TCPServerException("The pointer is invalid") {}
};

#endif
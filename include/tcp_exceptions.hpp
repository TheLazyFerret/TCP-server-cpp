/**
 * @file tcp_.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Header file for exceptions. 
 *   Don't require to be included 
 */

#ifndef TCP_EXCEPTIONS_HPP
#define TCP_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <cstring>

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
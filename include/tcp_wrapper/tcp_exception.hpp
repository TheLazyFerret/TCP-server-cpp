/**
 * @file tcp_exception.hpp
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief Exceptions definition.
 *   Don't require to be included 
 */

#ifndef TCP_WRAPPER_EXCEPTION_HPP
#define TCP_WRAPPER_EXCEPTION_HPP

#include <exception>
#include <string>
#include <cstring>

namespace tcp_exception {

/// @brief Base class for representing all the exceptions of the class library.
class TCPException : public std::exception {
  public:
    TCPException(const std::string& message) : error_(message) {}
    const char* what() const noexcept override {return error_.c_str();}
  private:
    std::string error_;
};

/// @brief Meta exception, that represents all error where errno is present.
class ErrnoException : public TCPException {
  public: 
    ErrnoException(const int error_code) : TCPException(std::string(std::strerror(error_code))) {}
};

/// @brief Exception called when tried to convert an invalid address.
class ConvertBinaryException : public TCPException {
  public:
    ConvertBinaryException(const std::string& address) : TCPException("Invalid address: " + address) {}
};

/// @brief Exception representing situations where trying to access a recurse while object not initialized correctly
class NotInitialized : public TCPException {
  public:
    NotInitialized() : TCPException("Trying to access a method while object not correctly initialized") {}
};

/// @brief Exception sent when a pointer is invalid.
///   Basically trying to avoid a seg fault.
class InvalidPointer : public TCPException {
  public:
    InvalidPointer() : TCPException("The pointer is invalid") {}
};

}

#endif
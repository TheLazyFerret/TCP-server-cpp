/**
 * @file tcp_common.coo
 * @author TheLazyFerret (https://github.com/TheLazyFerret)
 * @copyright (c) 2025 TheLazyFerret
 *  Licensed under MIT License. See LICENSE file in the project root for full license information.
 * 
 * @brief implementation of common functions used by both libraries.
 */

#include "tcp_internal.hpp"
#include "tcp_exception.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <iostream>

using namespace tcp_exception;

/// @brief Convert a string IPv4 addr to binary (in ip format).
/// @param address 
/// @return the address in binary as a in_addr struct.
in_addr tcp_internal::ConvertAddrBinary(const std::string& address) {
  in_addr addr;
  memset(&addr, 0, sizeof(addr));
  const int result = inet_pton(AF_INET, address.c_str(), &addr);
  if (result == 0) {
    throw ConvertBinaryException(address);
  }
  else if (result < 0) {
    throw ErrnoException(errno);
  }
  return addr;
}

/// @brief Convert a binary address to a human redeable string address.
/// @param address 
/// @return the address as a std::string.
std::string tcp_internal::ConvertAddrSring(const in_addr& address) {
  char str[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &address, str, INET_ADDRSTRLEN) == nullptr) {
    throw ErrnoException(errno);
  }
  const std::string result(str);
  return result;
}
# Begin
cmake_minimum_required(VERSION 3.20)
project(tcp_wrapper_cpp)

# Set libraries name
set(TCP_SERVER_LIBNAME tcp_server) # SERVER
set(TCP_CLIENT_LIBNAME tcp_client) # CLIENT

# Create the library.
add_library(${TCP_SERVER_LIBNAME} STATIC) # SERVER
add_library(${TCP_CLIENT_LIBNAME} STATIC) # CLIENT

# Compiling definitions
target_compile_features(${TCP_SERVER_LIBNAME} PUBLIC cxx_std_20) # SERVER
target_compile_features(${TCP_CLIENT_LIBNAME} PUBLIC cxx_std_20) # CLIENT

# Debug printing.
option(DEBUG_MODE "Enable debug messages" ON)
if (DEBUG_MODE)
  target_compile_definitions(${TCP_SERVER_LIBNAME} PRIVATE DEBUG=1) # SERVER
  target_compile_definitions(${TCP_CLIENT_LIBNAME} PRIVATE DEBUG=1) # CLIENT
endif()

target_sources(${TCP_SERVER_LIBNAME} # SERVER
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/tcp_internal.cpp
    ${CMAKE_SOURCE_DIR}/src/tcp_server.cpp
)

target_sources(${TCP_CLIENT_LIBNAME} # CLIENT
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/tcp_internal.cpp
    ${CMAKE_SOURCE_DIR}/src/tcp_client.cpp
)

target_include_directories(${TCP_SERVER_LIBNAME} # SERVER
  PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(${TCP_CLIENT_LIBNAME} # CLIENT
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include    
)


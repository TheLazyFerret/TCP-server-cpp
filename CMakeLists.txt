# Begin
cmake_minimum_required(VERSION 3.20)
project(TCPServer)

# Create the library.
add_library(${PROJECT_NAME} STATIC)

# Compiling definitions
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Debug printing.
option(DEBUG_MODE "Enable debug messages" ON)
if (DEBUG_MODE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
endif()

# Add source files.
target_sources(${PROJECT_NAME} 
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_server_connection.cpp
)

# Add the include files.
target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
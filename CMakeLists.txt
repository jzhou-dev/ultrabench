cmake_minimum_required(VERSION 3.16)
project(veloxy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(veloxy
  src/cli/cli.cpp
  src/core/help.cpp
  src/core/run.cpp
)

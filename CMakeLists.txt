cmake_minimum_required(VERSION 3.16)
project(ultrabench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ultrabench
  src/cli/cli.cpp
  src/core/help.cpp
  src/core/run.cpp
  src/core/http.cpp
  src/core/compare.cpp
  src/core/file.cpp
  src/core/db.cpp
  src/core/load.cpp
)

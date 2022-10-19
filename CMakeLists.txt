cmake_minimum_required(VERSION 3.10)

project(pan)

set(CMAKE_CXX_STANDARD 17)

include_directories(json/)
file(GLOB JSON json/json.cpp json/Parser.cpp)

add_executable(pan main.cpp ${JSON})
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(zubr-hardware-inteface)

include_directories(Inc/)

add_library(BaseSerial Src/BaseSerial.cpp)
add_library(CsMessage Src/CsMessage.cpp)

add_executable(test Src/Test.cpp)
target_link_libraries(test PRIVATE BaseSerial CsMessage)
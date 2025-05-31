cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

project(zubr-sdk)

include_directories(Inc/)

#add_library(BaseSerial Src/BaseSerial.cpp)
#add_library(CsMessage Src/CsMessage.cpp)

#add_library(ZubrIO Src/ZubrIO.cpp)
#target_link_libraries(ZubrIO PUBLIC BaseSerial CsMessage)

#add_library(ZubrRPC Src/ZubrRPC.cpp)
#target_link_libraries(ZubrRPC PUBLIC ZubrIO)

file( GLOB SRCS Src/*)

add_library(ZubrSDK ${SRCS})
target_include_directories(ZubrSDK PUBLIC Inc/)

#add_executable(test Src/Test.cpp)
#target_link_libraries(test PRIVATE BaseSerial CsMessage)
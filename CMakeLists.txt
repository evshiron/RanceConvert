cmake_minimum_required(VERSION 3.3)
project(RanceConvert)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp formats/qnt.cpp formats/qnt.h common.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(RanceConvert ${SOURCE_FILES})

target_link_libraries(RanceConvert z)
cmake_minimum_required(VERSION 2.8)
project(_lightredis)
aux_source_directory(. DIR_SRCS)
add_definitions(-std=c++11)

add_subdirectory(command)

add_subdirectory(data)

add_subdirectory(reactor)

add_executable(lightredis ${DIR_SRCS})
target_link_libraries(lightredis reactor command data)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

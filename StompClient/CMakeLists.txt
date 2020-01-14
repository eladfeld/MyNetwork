cmake_minimum_required(VERSION 3.15)
project(StompClient)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS filesystem regex)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(
            StompClient
            main.cpp
            include/StompMessage.h
            include/StompBookClubClient.h
            src/StompMessage.cpp
            src/StompBookClubClient.cpp
            src/ConnectionHandler.cpp
            include/ConnectionHandler.h)

    target_link_libraries(StompClient ${Boost_LIBRARIES})
endif()


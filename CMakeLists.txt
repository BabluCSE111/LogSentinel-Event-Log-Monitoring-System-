cmake_minimum_required(VERSION 3.15)

project(LogSentinel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LogSentinel
    src/main.cpp
    src/Event.cpp
    src/LogReader.cpp
    src/EventMonitor.cpp
    src/Config.cpp
)

target_include_directories(LogSentinel PRIVATE
    include
)

if(WIN32)
    target_link_libraries(LogSentinel PRIVATE wevtapi)
endif()
cmake_minimum_required(VERSION 3.9.1)
project(AlarmPlayer)
set(CMAKE_CXX_STANDARD 11)
if(UNIX)
    message("This is a ${CMAKE_SYSTEM_NAME} system")
elseif(WIN32)
    message("This is a Windows System (not tested)")
endif()


add_executable(alarm_player
        src/Alarm.h
        src/Alarm.cpp
        src/main.cpp)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(alarm_player PRIVATE Threads::Threads)

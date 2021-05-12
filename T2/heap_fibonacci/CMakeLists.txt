cmake_minimum_required(VERSION 3.15)
project(tsp)

set(CMAKE_C_STANDARD 11)

include_directories(.)


add_executable(tsp
        tsp.h
        tsp.cpp
        )
target_link_libraries(tsp m rt)
        
        

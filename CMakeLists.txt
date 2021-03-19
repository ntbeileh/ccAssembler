cmake_minimum_required(VERSION 3.18)

set(This confusedContigsAssembler)

project(${This} CXX)



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
enable_testing()

add_subdirectory(googletest)

set(Headers 
    node.h
    repeatGraph.h
    overlapAlign.h)

set(Sources
    node.cpp
    repeatGraph.cpp
    main.cpp
    preprocessing.cpp
    overlapAlign.cpp)

add_library(${This} STATIC ${Sources} ${Headers})


add_subdirectory(test)
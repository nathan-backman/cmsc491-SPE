cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(spe
    VERSION 0.1.1
    DESCRIPTION "Stream Processing Engine developed BVU CMSC491 Fall 2019"
    LANGUAGES C CXX
    )



set(spe_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(spe_BINARY_DIR "${CMAKE_BINARY_DIR}/bin")
set(spe_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/include")
set(spe_TESTS_DIR "${CMAKE_SOURCE_DIR}/tests")



add_library(spe STATIC
    src/Data/Data.cpp
    src/Operators/Operator.cpp
    src/Operators/InputSource.cpp
    src/Operators/Emitter.cpp
    src/Engine/StreamProcessingEngine.cpp
    )

set_target_properties(spe PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER src/include/SPE.h
    LIBRARY_OUTPUT_NAME silly
    )


target_include_directories(spe PRIVATE src/include)
  
include(GNUInstallDirs)

# Output directory.
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fmax-errors=1")



################################################################################
# DEPENDENCIES
################################################################################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(spe Threads::Threads)

add_subdirectory(apps)
add_subdirectory(tests)

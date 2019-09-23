cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(spe
    VERSION 0.1.1
    DESCRIPTION "Stream Processing Engine developed BVU CMSC491 Fall 2019"
    LANGUAGES C CXX
    )

message("CMAKE_CXX_COMPILER_ID: [${CMAKE_CXX_COMPILER_ID}]")


set(spe_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(spe_BINARY_DIR "${CMAKE_BINARY_DIR}/bin")
set(spe_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/include")
set(spe_TESTS_DIR "${CMAKE_SOURCE_DIR}/tests")


if(MSVC)
  add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
  add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
endif()


add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

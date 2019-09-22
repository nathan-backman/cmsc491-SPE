cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(spe
    VERSION 0.1.1
    DESCRIPTION "Stream Processing Engine developed BVU CMSC491 Fall 2019"
    LANGUAGES C CXX
    )

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
    )

#configure_file(spe.pc.in spe.pc @ONLY)

target_include_directories(spe PRIVATE src/include)
  
include(GNUInstallDirs)

install(TARGETS spe
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/archive
    )

#install(FILES ${CMAKE_BINARY_DIR}/spe.pc
#        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


# Compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fmax-errors=1")



################################################################################
# DEPENDENCIES
################################################################################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(spe Threads::Threads)



add_library(spe STATIC
    ${spe_SOURCE_DIR}/Data/Data.cpp
    ${spe_SOURCE_DIR}/Operators/Operator.cpp
    ${spe_SOURCE_DIR}/Operators/InputSource.cpp
    ${spe_SOURCE_DIR}/Operators/Emitter.cpp
    ${spe_SOURCE_DIR}/Engine/StreamProcessingEngine.cpp
    )

set_target_properties(spe PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER ${spe_INCLUDE_DIR}/SPE.h
    )


target_include_directories(spe PRIVATE ${spe_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(spe Threads::Threads)


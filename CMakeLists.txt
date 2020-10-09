cmake_minimum_required(VERSION 3.10)

project(Jacy VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(Jacy PRIVATE ${PROJECT_SOURCE_DIR}/include)

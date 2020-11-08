cmake_minimum_required(VERSION 3.10)

project(Jacy VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(GCC_COVERAGE_COMPILE_FLAGS "-g3 -ggdb3 -coverage -fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} include/compiler/Scope.h include/compiler/types.h include/vm/BaseVM.h include/vm/Value.h src/vm/BaseVM.cpp include/vm/Disasm.h src/vm/Disasm.cpp include/vm/VM.h src/vm/VM.cpp include/vm/Verifier.h src/vm/Virifier.cpp include/vm/globals.h)

target_include_directories(Jacy PRIVATE ${PROJECT_SOURCE_DIR}/include)

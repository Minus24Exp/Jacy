cmake_minimum_required(VERSION 3.10)

project(Jacy VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GCC_COVERAGE_COMPILE_FLAGS "-g3 -ggdb3 -coverage -fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")

include_directories("${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} include/common/Logger.h include/common/Options.h include/common/Config.h include/compiler/type_class.h include/bytecode/constant.h include/compiler/type.h src/compiler/type_class.cpp include/common/Logger.inl include/vm/class.h src/vm/object.cpp include/tree/Expr/MethodCall.h src/vm/class.cpp include/tree/Expr/SpreadExpr.h src/common/Config.cpp include/tree/Type.h src/common/Logger.cpp include/tree/Expr/Grouping.h)

target_include_directories(Jacy PRIVATE ${PROJECT_SOURCE_DIR}/include)

cmake_minimum_required(VERSION 3.17)
project(tritset1)

add_subdirectory(lib/googletest-main)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
add_executable(tritset1 main.cpp Trit.cpp Trit.h TritSet.cpp TritSet.h TritSetFixtures.cpp)

target_link_libraries(tritset1 gtest gtest_main pthread)

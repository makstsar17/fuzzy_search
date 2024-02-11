cmake_minimum_required(VERSION 3.25)
project(Diplom)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

add_executable(Diplom main.cpp)

target_link_libraries(Diplom similarityTable)


cmake_minimum_required(VERSION 3.10)
project(test_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(test main.cpp)
target_include_directories(test PRIVATE ../)
# target_link_libraries(MIAN PRIVATE jsoncpp)

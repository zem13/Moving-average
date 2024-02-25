cmake_minimum_required(VERSION 2.8)

project(TestCase)

add_compile_options(-std=c++11)

add_executable(moving_avg src/main.cpp)
target_include_directories(moving_avg PUBLIC Include)
target_link_libraries(moving_avg pthread)

cmake_minimum_required(VERSION 3.10)
project(CH2_EX2)

find_package(Eigen3 REQUIRED)

add_executable(main 
    find_vol.cpp
    math.cpp
)

target_link_libraries(main Eigen3::Eigen)
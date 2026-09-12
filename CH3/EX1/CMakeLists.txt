cmake_minimum_required(VERSION 3.10)
project(CH3_EX1)

find_package(Eigen3 REQUIRED)

add_executable(main
    main.cpp
    diagonalize.cpp
)

target_link_libraries(main Eigen3::Eigen)
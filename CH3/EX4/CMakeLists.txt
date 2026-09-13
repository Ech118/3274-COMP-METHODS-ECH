cmake_minimum_required(VERSION 3.10)
project(CH3_EX4)

find_package(Eigen3 REQUIRED)

add_executable(main
    main.cpp
    schrodinger.cpp
)

target_link_libraries(main Eigen3::Eigen)

cmake_minimum_required(VERSION 3.16)
project(C3_EX5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Eigen3 REQUIRED)
find_package(Qat REQUIRED)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_BUILD_RPATH "${QatPlotWidgets_DIR}/../..")

add_executable(main
    main.cpp
    oscillator.cpp
)

target_link_libraries(main PRIVATE Eigen3::Eigen Qat::QatPlotWidgets Qat::QatPlotting Qat::QatGenericFunctions Qt6::Widgets Qt6::Core)

cmake_minimum_required(VERSION 3.12)
project(OPP)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
add_executable(OPP main.cpp)
target_link_libraries(OPP ${OpenCV_LIBS})
cmake_minimum_required(VERSION 3.12)
project(OVP)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
add_executable(OVP main.cpp)
target_link_libraries(OVP ${OpenCV_LIBS})
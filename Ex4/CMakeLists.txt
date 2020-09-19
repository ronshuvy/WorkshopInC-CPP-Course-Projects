cmake_minimum_required(VERSION 3.15)
project(Ex4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ex4 main.cpp Dense.h Dense.cpp Activation.cpp Matrix.cpp MlpNetwork.cpp driver.cpp)
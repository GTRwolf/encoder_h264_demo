cmake_minimum_required(VERSION 3.5)
project(encode_h264_demo)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(encode_h264_demo main.cpp encoder.cpp encoder.h)

target_link_libraries(encode_h264_demo libavutil.so libavcodec.so libavformat.so libswscale.so ${OpenCV_LIBS})
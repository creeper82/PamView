cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
add_library(
    pamview_library
    pixel.cpp pixel.h
    bitmap.cpp bitmap.h
    parser.cpp parser.h
    transformations.cpp transformations.h
    color.cpp color.h
    exceptions.h
)
target_include_directories(pamview_library PUBLIC include)
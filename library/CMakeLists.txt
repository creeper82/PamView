cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
add_executable(
    pamview
    main.cpp
    pixel.cpp pixel.h
    bitmap.cpp bitmap.h
    parser.cpp parser.h
    transformations.cpp transformations.h
    color.cpp color.h
    exceptions.h
)
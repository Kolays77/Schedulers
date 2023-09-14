cmake_minimum_required (VERSION 3.27)
project (schedulers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app main.cpp)

add_subdirectory(src)

target_link_libraries(app PRIVATE src)

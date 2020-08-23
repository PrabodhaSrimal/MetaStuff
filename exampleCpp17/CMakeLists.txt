cmake_minimum_required(VERSION 3.16)
project(MetaStuffTestCpp17)
# Set C++ version for compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(exe_name meta-stuff-test-cpp17)

add_executable(${exe_name} main.cpp)
target_include_directories(${exe_name} PRIVATE
    nlohmann_json # json in subdir
)

target_sources(${exe_name} PRIVATE
    StringCast.cpp
    StringCast.h
    JsonCast.h
    JsonCast.inl
)

target_link_libraries(${exe_name} PRIVATE MetaStuff)


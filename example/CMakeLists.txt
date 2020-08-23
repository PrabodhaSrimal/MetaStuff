cmake_minimum_required(VERSION 3.0)
project(MetaStuffTest)

set(exe_name meta-stuff-test)

add_executable(${exe_name} main.cpp)
target_include_directories(${exe_name} PRIVATE
    nlohmann_json # json in subdir
)

target_sources(${exe_name} PRIVATE
    StringCast.cpp
    StringCast.h
    JsonCast.h
    MovieInfo.h
    Person.h
    JsonCast.inl
)

target_link_libraries(${exe_name} PRIVATE MetaStuff)


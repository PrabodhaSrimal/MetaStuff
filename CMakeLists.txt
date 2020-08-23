cmake_minimum_required(VERSION 3.16)
project(MetaStuff VERSION 0.0.1 LANGUAGES CXX)

# Set C++ version for compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(MetaStuff INTERFACE)
target_include_directories(MetaStuff INTERFACE include/)

# build examples
option(WITH_EXAMPLES "Build examples" ON)
if(WITH_EXAMPLES)
    add_subdirectory(example)
endif()

# install step
include(GNUInstallDirs)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MetaStuff
)


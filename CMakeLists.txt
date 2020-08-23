cmake_minimum_required(VERSION 3.0)
project(MetaStuff VERSION 0.0.1 LANGUAGES CXX)

add_library(MetaStuff INTERFACE)
target_include_directories(MetaStuff INTERFACE include/)

# build examples
option(WITH_EXAMPLES "Build examples" OFF)
if(WITH_EXAMPLES)
    add_subdirectory(example)
endif()

# install step
include(GNUInstallDirs)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MetaStuff
)


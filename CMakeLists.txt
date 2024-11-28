cmake_minimum_required(VERSION 3.15)

set(VERSION)
file(STRINGS VERSION VERSION)

project(AwesomeApp
    DESCRIPTION "Awesome application"
    LANGUAGES CXX
    VERSION ${VERSION})

set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "CMake module path" FORCE)
include(Utils)

add_subdirectory(src)

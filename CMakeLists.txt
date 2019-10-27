# 
cmake_minimum_required(VERSION 2.8.9)
project(Whizzles)

# compile flags
set(CMAKE_CXX_FLAGS "-Wall")

# Version
set(Whizzles_VERSION_MAJOR 1)
set(Whizzles_VERSION_Minor 0)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include headers
include_directories(include)
include_directories(src)

# add Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "test/*.cpp")


# link against sfml
find_package(SFML COMPONENTS graphics window network REQUIRED)
add_executable(Whizzles ${SOURCES})

target_link_libraries(Whizzles sfml-graphics sfml-window sfml-network)

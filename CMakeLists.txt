cmake_minimum_required(VERSION 2.8.9)
project(Whizzles)

# compile flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")

# Version
set(Whizzles_VERSION_MAJOR 1)
set(Whizzles_VERSION_Minor 0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include headers
include_directories(src)
include_directories(src/state)
include_directories(src/manager)
include_directories(src/userinterface)
include_directories(src/utility)
include_directories(src/network)
include_directories(src/gamelogic)

# add Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(SFML COMPONENTS REQUIRED graphics window network audio REQUIRED)
add_executable(Whizzles ${SOURCES})
target_link_libraries(Whizzles sfml-graphics sfml-window sfml-network sfml-audio -lboost_random)

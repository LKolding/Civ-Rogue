cmake_minimum_required(VERSION 3.10)
project(CivRogue)


# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define directories
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

# Include headers
include_directories(${INCLUDE_DIR})

# Find sfml
set(SFML_STATIC_LIBRARIES FALSE)
set(SFML_DIR ${LIB_DIR}/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Gather source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

# add exe
add_executable(${PROJECT_NAME} ${SOURCES})

# link sfml libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

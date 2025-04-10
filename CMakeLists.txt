cmake_minimum_required(VERSION 3.10)
project(CivRogue)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define directories
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH ${LIB_DIR}/sfml/lib)

# Include headers
include_directories(${INCLUDE_DIR})
include_directories(${LIB_DIR}/sfml/include)

# Find sfml
#set(SFML_STATIC_LIBRARIES FALSE)
#set(SFML_DIR ${LIB_DIR}/sfml)
#find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Gather source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE CSOURCES ${SRC_DIR}/*.c)

# add exe
add_executable(${PROJECT_NAME} ${SOURCES} ${CSOURCES})

# link sfml libraries
#target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# link sfml dylibs
target_link_libraries(${PROJECT_NAME} 
    ${LIB_DIR}/sfml/lib/libsfml-graphics.dylib
    ${LIB_DIR}/sfml/lib/libsfml-window.dylib
    ${LIB_DIR}/sfml/lib/libsfml-system.dylib
    "-framework OpenGL"
)
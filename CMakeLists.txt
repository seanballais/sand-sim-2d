################################################################################
# Set up CMake.                                                                #
################################################################################
cmake_minimum_required(VERSION 3.1)
project(sand-sim-2d)

# Integrate with Conan.
include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

################################################################################
# Set up compilation of the app.                                               #
################################################################################
set(PROJECT_MAIN "${CMAKE_SOURCE_DIR}/main.cpp")

add_executable(sand-sim-2d ${PROJECT_MAIN})
target_link_libraries(sand-sim-2d ${CONAN_LIBS})

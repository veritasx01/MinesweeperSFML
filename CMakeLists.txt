cmake_minimum_required(VERSION 3.28)
project(minesweeper LANGUAGES CXX)

# Set the output directory for the compiled binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# FetchContent to download and build SFML
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

# Add the main executable and all source files
add_executable(minesweeper 
    src/main.cpp 
    src/mines_logic.cpp 
    src/mines_logic.h
)

# Set C++ standard to 17
target_compile_features(minesweeper PRIVATE cxx_std_17)

# Link the SFML libraries manually from the SFML build directory
target_link_libraries(minesweeper PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

add_custom_command(
    TARGET minesweeper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/assets
)
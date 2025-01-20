cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(minesweeper)
add_executable(minesweeper
    src/main.cpp
    src/mines_logic.cpp
)
find_package(SFML 2.6 REQUIRED graphics window system)
target_link_libraries(minesweeper
    sfml-graphics
    sfml-window
    sfml-system
)
target_include_directories(minesweeper PRIVATE src)
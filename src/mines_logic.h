#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#ifndef GAME_STATE_H
#define GAME_STATE_H

struct gameState {
    std::vector<std::vector<sf::RectangleShape>> grid;
    std::vector<std::vector<char>> originalBoard;
    std::vector<std::vector<char>> board;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> flaged;
    std::string difficulty;
    int rows;
    int cols;
    int mines;
    int x;
    int y;
    bool isFirstClick;
    bool win;
    bool gameOver;
};

#endif
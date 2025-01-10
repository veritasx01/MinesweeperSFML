#include "mines_logic.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using Config = std::tuple<int, int, int>; // (rows, cols, mines)

Config getBoardConfig(const string &difficulty) {
    // Create a map of difficulty levels and their respective configurations
    std::map<std::string, Config> configs = {{"BEGINNER", {9, 9, 10}},
                                             {"INTERMEDIATE", {16, 16, 40}},
                                             {"EXPERT", {16, 30, 99}}};

    auto it = configs.find(difficulty);
    if (it != configs.end()) {
        return it->second;
    }

    return {0, 0, 0};
}

// Beginner is usually on an 8x8 or 9x9 board containing 10 mines,
// Intermediate is usually on a 16x16 board with 40 mines and expert is usually
// on a 30x16 board with 99 mines
int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

void EvaluateBoardCells(vector<vector<char>> &board) {
    int rows = board.size();
    int cols = board[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == 'M') {
                continue;
            }
            int cnt = 0;
            for (int k = 0; k < 8; k++) {
                int idx = i + dx[k];
                int jdx = j + dy[k];
                if (idx >= 0 && idx < rows && jdx >= 0 && jdx <= cols &&
                    board[idx][jdx] == 'M') {
                    cnt++;
                }
            }
            board[i][j] = '0' + cnt;
        }
    }
}

vector<vector<char>> GenBoard(int rows, int cols, int mines) {
    vector<vector<char>> board =
        vector<vector<char>>(rows, vector<char>(cols, '0'));
    srand(time(0));
    int placedMines = 0;
    while (placedMines < mines) {
        int randRow = rand() % rows;
        int randCol = rand() % cols;

        if (board[randRow][randCol] != 'M') {
            board[randRow][randCol] = 'M';
            placedMines++;
        }
    }
    EvaluateBoardCells(board);
    return board;
}

void ProcessClickOnZero(vector<vector<char>> &board,
                        vector<vector<bool>> &visited, int i, int j) {
    // processing a click on a non zero is just setting visited[i][j] = 1
    // and on a mine is better handeled in main
    if (!(i >= 0 && i < visited.size() && j >= 0 && j < visited[i].size() &&
          !visited[i][j])) {
        return;
    }
    visited[i][j] = 1;
    if (board[i][j] == '0') {
        ProcessClickOnZero(board, visited, i + 1, j);
        ProcessClickOnZero(board, visited, i - 1, j);
        ProcessClickOnZero(board, visited, i, j + 1);
        ProcessClickOnZero(board, visited, i, j - 1);

        ProcessClickOnZero(board, visited, i + 1, j + 1);
        ProcessClickOnZero(board, visited, i - 1, j - 1);
        ProcessClickOnZero(board, visited, i - 1, j + 1);
        ProcessClickOnZero(board, visited, i + 1, j - 1);
    }
}

void chord(vector<vector<char>> &board, vector<vector<bool>> &visited,
           vector<vector<bool>> &flagged, int i, int j) {
    char tile = board[i][j];
    if (tile == 'M' || tile == '0' || !visited[i][j]) {
        return;
    }
    int rows = visited.size();
    int cols = visited[0].size();
    int cnt = 0;
    vector<pair<int,int>> nonFlaggedMines;
    for (int k = 0; k < 8; k++) {
        int idx = i + dx[k];
        int jdx = j + dy[k];
        bool inside = idx >= 0 && idx < rows && jdx >= 0 && jdx <= cols;
        cnt += (inside && board[idx][jdx] == 'M' && flagged[idx][jdx]);
        if (inside && board[idx][jdx] == 'M' && !flagged[idx][jdx]) {
            nonFlaggedMines.push_back({idx,jdx});
        }
    }
    if (nonFlaggedMines.size() > 0) {
        for (auto pair : nonFlaggedMines) {
            visited[pair.first][pair.second] = 1;
        }
        return;
    }
    if (cnt == tile - '0') {
        for (int k = 0; k < 8; k++) {
            int idx = i + dx[k];
            int jdx = j + dy[k];
            bool inside = idx >= 0 && idx < rows && jdx >= 0 && jdx <= cols;
            if (inside && !flagged[idx][jdx]) {
                if (board[idx][jdx] == '0') {
                    ProcessClickOnZero(board, visited, idx, jdx);
                } else {
                    visited[idx][jdx] = 1;
                }
            }
        }
    }
}

string BoardToString(vector<vector<char>> &board) {
    int rows = board.size();
    int cols = board[0].size();
    string res = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res += board[i][j];
        }
        res += "\n";
    }
    return res;
}

string BoardToString(vector<vector<bool>> &board) {
    int rows = board.size();
    int cols = board[0].size();
    string res = "";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res += board[i][j] ? '1' : '0';
        }
        res += "\n";
    }
    return res;
}

bool IsWinCondition(vector<vector<char>> &board,
                    vector<vector<bool>> &visited) {
    int rows = board.size();
    int cols = board[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] != 'M' && !visited[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void OneShotProtection(gameState &gs, int idx, int jdx) {
    if (gs.board[idx][jdx] != 'M' || !gs.isFirstClick) {
        gs.isFirstClick = false;
        return;
    }
    gs.originalBoard = gs.board;
    gs.isFirstClick = false;
    cout << "OSP!\n";
    for (int i = 0; i < gs.rows; i++) {
        for (int j = 0; j < gs.cols; j++) {
            if (gs.board[i][j] != 'M') {
                gs.board[i][j] = 'M';
                gs.board[idx][jdx] = '0';
                EvaluateBoardCells(gs.board);
                return;
            }
        }
    }
}

void SquareClick(gameState &gs, int i, int j, sf::Vector2f clickPosition,
                 bool left, bool right) {
    if (gs.grid[i][j].getGlobalBounds().contains(clickPosition)) {
        std::cout << "Square clicked at (" << i << ", " << j << ")\n";
        if (gs.visited[i][j]) {
            chord(gs.board, gs.visited, gs.flaged, i, j);
        } else if (right && (!gs.visited[i][j] || gs.flaged[i][j])) {
            gs.flaged[i][j] = !gs.flaged[i][j];
        } else if (gs.board[i][j] == '0') {
            ProcessClickOnZero(gs.board, gs.visited, i, j);
        } else {
            OneShotProtection(gs, i, j);
            if (gs.board[i][j] == '0') {
                ProcessClickOnZero(gs.board, gs.visited, i, j);
            }
            gs.visited[i][j] = 1;
        }
    }
}

void HandleLeftRight(sf::RenderWindow &window, gameState &gs,
                     sf::Event &event) {
    bool left = event.mouseButton.button == sf::Mouse::Left;
    bool right = event.mouseButton.button == sf::Mouse::Right;
    auto mouse_pos = sf::Mouse::getPosition(window);
    auto clickPosition = window.mapPixelToCoords(mouse_pos);

    // Check if any rectangle is clicked
    for (int i = 0; i < gs.rows; i++) {
        for (int j = 0; j < gs.cols; j++) {
            SquareClick(gs, i, j, clickPosition, left, right);
        }
    }
}

void InitializeGrid(gameState &gs, const sf::Vector2f &rectSize,
                    sf::Texture &texture, const float SPACING,
                    const float OUTLINE_THICKNESS) {
    for (int i = 0; i < gs.rows; i++) {
        std::vector<sf::RectangleShape> v1;
        for (int j = 0; j < gs.cols; j++) {
            sf::RectangleShape rect(rectSize);
            rect.setOutlineThickness(OUTLINE_THICKNESS);
            rect.setOutlineColor(sf::Color(0, 0, 0));
            rect.setTexture(&texture);
            int ipos = i * (rectSize.y + SPACING + OUTLINE_THICKNESS);
            int jpos = j * (rectSize.x + SPACING + OUTLINE_THICKNESS);
            rect.setPosition(jpos, ipos);
            v1.push_back(rect);
        }
        gs.grid.push_back(v1);
    }
}

void InitialGameState(gameState &gs, string difficulty) {
    gs.grid.clear(); // Clear the existing grid
    gs.grid.shrink_to_fit();

    gs.difficulty = difficulty;
    std::tie(gs.rows, gs.cols, gs.mines) = getBoardConfig(gs.difficulty);
    gs.board = GenBoard(gs.rows, gs.cols, gs.mines);
    gs.originalBoard = gs.board;
    gs.visited = vector<vector<bool>>(gs.rows, vector<bool>(gs.cols, 0));
    gs.flaged = vector<vector<bool>>(gs.rows, vector<bool>(gs.cols, 0));
    gs.gameOver = false;
    gs.win = false;
    gs.isFirstClick = true;
    gs.x = -1;
    gs.y = -1;
}

void ResetBoard(gameState &gs) {
    gs.board = gs.originalBoard;
    gs.x = -1;
    gs.y = -1;
    gs.isFirstClick = true;
    gs.win = false;
    gs.gameOver = false;
    gs.visited = vector<vector<bool>>(gs.rows, vector<bool>(gs.cols, 0));
    gs.flaged = vector<vector<bool>>(gs.rows, vector<bool>(gs.cols, 0));
}

void HandleClicks(sf::RenderWindow &window, gameState &gs, sf::Texture &texture,
                  const float SPACING, const float OUTLINE_THICKNESS) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            HandleLeftRight(window, gs, event);
        } else if (event.type == sf::Event::KeyPressed) {
            auto eventKey = event.key.code;
            if (eventKey == sf::Keyboard::B) {
                InitialGameState(gs, "BEGINNER");
                InitializeGrid(gs, sf::Vector2f(25.0f, 25.0f), texture, SPACING,
                               OUTLINE_THICKNESS);
            } else if (eventKey == sf::Keyboard::I) {
                InitialGameState(gs, "INTERMEDIATE");
                InitializeGrid(gs, sf::Vector2f(25.0f, 25.0f), texture, SPACING,
                               OUTLINE_THICKNESS);
            } else if (eventKey == sf::Keyboard::E) {
                InitialGameState(gs, "EXPERT");
                InitializeGrid(gs, sf::Vector2f(25.0f, 25.0f), texture, SPACING,
                               OUTLINE_THICKNESS);
            } else if (eventKey == sf::Keyboard::R) {
                ResetBoard(gs);
            } else if (eventKey == sf::Keyboard::Escape ||
                       eventKey == sf::Keyboard::Q) {
                window.close();
            }
        }
    }
}

void ProcessChange(gameState &gs, map<char, sf::Texture> &tileMap,
                   sf::Texture &texture, int idx, int jdx) {
    char tileValue = gs.board[idx][jdx];
    if (gs.flaged[idx][jdx]) {
        gs.grid[idx][jdx].setTexture(&tileMap['F']);
    } else if (gs.visited[idx][jdx]) {
        gs.grid[idx][jdx].setTexture(&tileMap[tileValue]);
        if (gs.board[idx][jdx] == 'M') {
            gs.gameOver = true;
            gs.x = idx;
            gs.y = jdx;
        }
    } else {
        gs.grid[idx][jdx].setTexture(&texture);
    }
}

void ProcessChanges(gameState &gs, map<char, sf::Texture> &tileMap,
                    sf::Texture &texture) {
    for (int idx = 0; idx < gs.rows; idx++) {
        for (int jdx = 0; jdx < gs.cols; jdx++) {
            ProcessChange(gs, tileMap, texture, idx, jdx);
        }
    }
}

void RenderCell(gameState &gs, map<char, sf::Texture> &tileMap, int idx,
                int jdx) {
    if (gs.board[idx][jdx] == 'M' && !(idx == gs.x && jdx == gs.y)) {
        gs.grid[idx][jdx].setTexture(&tileMap['R']);
    }
}

void RenderGameOverGrid(gameState &gs, map<char, sf::Texture> &tileMap) {
    for (int idx = 0; idx < gs.rows; idx++) {
        for (int jdx = 0; jdx < gs.cols; jdx++) {
            RenderCell(gs, tileMap, idx, jdx);
        }
    }
    if (!gs.win) {
        gs.grid[gs.x][gs.y].setTexture(&tileMap['M']);
    }
}

// int main() {
//     int n = 9;
//     int m = 9;
//     int mines = 10;
//     auto bo = GenBoard(n,m,mines);
//     vector<vector<bool>> visited(n,vector<bool>(m,0));
//     cout << BoardToString(bo);
//     ProcessClickOnZero(bo,visited,0,0);
//     cout << "-------------\n";
//     cout << BoardToString(visited);
//     return 0;
// }
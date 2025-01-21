#include "mines_logic.cpp"
#include "mines_logic.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void drawWinLoseScreen(sf::RenderWindow &window, bool isWin) {
    sf::Font font;
    if (!font.loadFromFile("Assets/OpenSans-VariableFont_wdth,wght.ttf")) {
        cout << "font missing from assets\n";
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(35);
    text.setFillColor(sf::Color::Green);
    if (isWin) {
        text.setString("You Win!");
    } else {
        text.setFillColor(sf::Color::Red);
        text.setString("Game Over!");
    }

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2, textRect.height / 0.5);
    text.setPosition(window.getSize().x / 2, window.getSize().y / 1.2);
    window.draw(text);
}

const float WIDTH = 1000; // SCREEN WIDTH
const float HEIGHT = 600; // SCREEN HEIGHT
const float SPACING = 0.0f;
const float OUTLINE_THICKNESS = 0.0f;
map<char, string> tileDict = {
    {'0', "Assets/tile000.jpg"}, {'1', "Assets/tile001.jpg"},
    {'2', "Assets/tile002.jpg"}, {'3', "Assets/tile003.jpg"},
    {'4', "Assets/tile004.jpg"}, {'5', "Assets/tile005.jpg"},
    {'6', "Assets/tile006.jpg"}, {'7', "Assets/tile007.jpg"},
    {'8', "Assets/tile008.jpg"}, {'M', "Assets/tile013.jpg"},
    {'F', "Assets/tile010.jpg"}, {'R', "Assets/tile012.jpg"}
};

int main() {
    gameState gs;
    gs.difficulty = "BEGINNER";
    InitialGameState(gs, gs.difficulty);
    const sf::Vector2f rectSize(25.0f, 25.0f); // Rectangle size (square)
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");
    sf::Text instructionText;
    sf::Font font;
    if (!font.loadFromFile("Assets/OpenSans-VariableFont_wdth,wght.ttf")) {
        cout << "font missing from assets\n";
    }
    instructionText.setFont(font);
    instructionText.setCharacterSize(20);
    instructionText.setString("Buttons:\n B: Beginner, I :Intermediate, E: Expert\n Q/ESC: quit, R: Reset");
    sf::FloatRect textInsRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textInsRect.width / 2, textInsRect.height / 0.5);
    instructionText.setPosition(window.getSize().x / 5.2, window.getSize().y / 1.1);
    instructionText.setFillColor(sf::Color::Cyan);

    sf::Texture texture;
    map<char, sf::Texture> tileMap;
    if (!texture.loadFromFile("Assets/tile009.jpg")) {
        std::cout << "texture not found\n";
    }
    InitializeGrid(gs, rectSize, texture, SPACING, OUTLINE_THICKNESS);

    for (auto [key, val] : tileDict) {
        sf::Texture txtr;
        txtr.loadFromFile(tileDict[key]);
        tileMap[key] = txtr;
    }

    while (window.isOpen()) {
        HandleClicks(window, gs, texture, SPACING, OUTLINE_THICKNESS);
        window.clear();

        if (!gs.gameOver) {
            ProcessChanges(gs, tileMap, texture);
        }

        // draw changes
        for (auto &idx : gs.grid) {
            for (auto &it : idx) {
                window.draw(it);
            }
        }

        gs.win = IsWinCondition(gs.board, gs.visited);
        gs.gameOver |= gs.win;
        if (gs.gameOver) {
            RenderGameOverGrid(gs, tileMap);
            drawWinLoseScreen(window, gs.win);
        }
        window.draw(instructionText);
        window.display();
    }
    return 0;
}

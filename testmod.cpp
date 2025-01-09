#include <SFML/Graphics.hpp>
#include <iostream>


sf::Vector2u WINDOW_SIZE{ 1400,900 };
constexpr unsigned TPS = 60; //ticks per seconds
const sf::Time     timePerUpdate = sf::seconds(1.0f / float(TPS));
sf::Vector2f TILE_SIZE{ 64.f,64.f };

struct Tile
{
        sf::RectangleShape shape;
};

#define DOUBLEARRAY

int main() {
        sf::RenderWindow window{ sf::VideoMode{WINDOW_SIZE.x,WINDOW_SIZE.y},"" };
        //      window.setFramerateLimit(60);
        window.setPosition(sf::Vector2i{ window.getPosition().x,0 });

        bool lostFocus = false;
        auto view = window.getDefaultView();

        sf::CircleShape shape{ TILE_SIZE.x / 3.f };
        shape.setPosition(static_cast<float>(WINDOW_SIZE.x) / 2.f,
                static_cast<float>(WINDOW_SIZE.y) / 2.f);
        shape.setFillColor({ 25,145,5 });
        shape.setOutlineColor(sf::Color::Black); shape.setOutlineThickness(1.5f);
        auto speed = 100.f;

        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        sf::Time FrameTime = sf::seconds(1.f / 60.f);

#ifdef SINGLEARRAY
        // Single Array
        std::vector<Tile> tilemap;
                for (auto y = 0; y < WINDOW_SIZE.y / TILE_SIZE.y; y++) {
        for (auto x = 0; x < WINDOW_SIZE.x / TILE_SIZE.x; x++) {
                        auto tile = new Tile();
                        tile->shape.setSize( {TILE_SIZE.x,TILE_SIZE.y} );
                        tile->shape.setFillColor({ 0,125,0,45 });
                        tile->shape.setOutlineThickness(0.5f);
                        tile->shape.setOutlineColor({ sf::Color::White });
                        tile->shape.setPosition(x * TILE_SIZE.x, y * TILE_SIZE.y);
                        tilemap.push_back(*tile);
                }
        }
#endif

#ifdef DOUBLEARRAY
        // Double Array
        std::vector<std::vector<Tile>> doubleTileMap;
                for (auto y = 0; y < WINDOW_SIZE.y / TILE_SIZE.y; y++) {
        for (auto x = 0; x < WINDOW_SIZE.x / TILE_SIZE.x; x++) {
                doubleTileMap.emplace_back();
                        auto tile = new Tile();
                        tile->shape.setSize({ TILE_SIZE.x,TILE_SIZE.y });
                        tile->shape.setFillColor({ 0,125,0,45 });
                        tile->shape.setOutlineThickness(0.5f);
                        tile->shape.setOutlineColor({ sf::Color::White });
                        tile->shape.setPosition(x * TILE_SIZE.x, y * TILE_SIZE.y);
                        doubleTileMap[y].push_back(*tile);
                }
        }
#endif

        while (window.isOpen()) {

                sf::Time dt = clock.restart();
                timeSinceLastUpdate += dt;

                        sf::Event event{};
                        while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                                        window.close();
                                if (event.type == sf::Event::KeyPressed) {
                                        switch (event.key.code) {
                                                case sf::Keyboard::Enter: std::cout << "Enter Pressed\n"; break;
                                                case sf::Keyboard::Space: std::cout << "Space Pressed\n"; break;
                                                default: break;
                                        }
                                }
                                if (event.type == sf::Event::Resized) {
                                        // update the view to the new size of the window
                                        view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                                }
                                if (event.type == sf::Event::LostFocus) { lostFocus = true; }
                                if (event.type == sf::Event::GainedFocus) { lostFocus = false; }
                        }

                        if (!lostFocus) {
                                auto mousePos = sf::Mouse::getPosition(window);
                                auto mouseWorldPos = window.mapPixelToCoords(mousePos, view);

                                window.setTitle("Mouse Position: (" + std::to_string(int(mouseWorldPos.x / 64.f)) + ", " +
                                        std::to_string(int(mouseWorldPos.y / 64.f)) + ")");

                                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                        if (mouseWorldPos.x >= 0 && mouseWorldPos.y >= 0 &&
                                                mouseWorldPos.x < WINDOW_SIZE.x && mouseWorldPos.y < WINDOW_SIZE.y) 
                                        {
                                                auto position = mousePos;
                                                position.x /= TILE_SIZE.x;
                                                position.y /= TILE_SIZE.y;


#ifdef SINGLEARRAY
                                                // Get 1D  (y * MapWidth + x) gives you 1D index
                                                auto index = position.y * (WINDOW_SIZE.x / TILE_SIZE.x) + position.x;
                                                tilemap[index].shape.setFillColor(sf::Color::Blue);
#endif


#ifdef DOUBLEARRAY
                                                doubleTileMap[position.y][position.x].shape.setFillColor(sf::Color::Red);

#endif
                                        }
                                }

                                //! ** INPUT SECTION **
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                                        shape.move(0.f, -speed * timePerUpdate.asSeconds());
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                                        shape.move(-speed * timePerUpdate.asSeconds(), 0.f);
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                                        shape.move(0.f, speed * timePerUpdate.asSeconds());
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                                        shape.move(speed * timePerUpdate.asSeconds(), 0.f);
                                }

                                while (timeSinceLastUpdate > FrameTime) {
                                        timeSinceLastUpdate -= FrameTime; while (timeSinceLastUpdate > FrameTime) {
                                                timeSinceLastUpdate -= FrameTime;
                                                //! ** UPDATE SECTION**

                                        }
                                }
                        }
                                                view.setCenter(shape.getPosition());
                                                window.setView(view);
                
                //! ** DRAW SECTION **
                window.clear();

#ifdef SINGLEARRAY
                for (auto& it : tilemap) {
                        window.draw(it.shape);
                }
#endif

#ifdef DOUBLEARRAY
                for (auto& index : doubleTileMap) {
                        for (auto& it : index) {

                window.draw(it.shape);
                        }
                }
#endif
                window.draw(shape);

                window.setView(window.getDefaultView());

                window.display();
        }

        return EXIT_SUCCESS;
}
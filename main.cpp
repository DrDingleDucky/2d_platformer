#include "player.h"
#include <fstream>
#include <iostream>
#include <sfml/Graphics.hpp>
#include <string>
#include <vector>

void loadLevel(std::string map, std::vector<Player> &playerGroup, std::vector<SolidTile> &solidTileGroup, std::vector<OneWayTile> &oneWayTileGroup) {
    std::ifstream file(map);
    std::string line;

    float x;
    float y;

    unsigned int column_index = 0;

    while (std::getline(file, line)) {
        for (unsigned int row_index = 0; row_index < line.length(); row_index++) {
            x = row_index * 36.0f;
            y = column_index * 36.0f;

            if (line[row_index] == 't') { // t - tile
                solidTileGroup.push_back(SolidTile(sf::Color(0, 0, 0), sf::Vector2f(36.0f, 36.0f), sf::Vector2f(x, y)));
            } else if (line[row_index] == 's') { // s - small tile
                solidTileGroup.push_back(SolidTile(sf::Color(0, 0, 0), sf::Vector2f(36.0f, 9.0f), sf::Vector2f(x, y)));
            } else if (line[row_index] == 'o') { // o - one way tile
                oneWayTileGroup.push_back(OneWayTile(sf::Color(139, 69, 19), sf::Vector2f(36.0f, 9.0f), sf::Vector2f(x, y)));
            } else if (line[row_index] == 'p') { // p - player
                playerGroup.push_back(Player(
                    3500.0f,                    // player acceleration
                    405.0f,                     // player max speed
                    2175.0f,                    // player gravity
                    -850.0f,                    // player jump velocity
                    3.0f,                       // player fall multiplier
                    5.0f,                       // player jump fall multiplier
                    1085.0f,                    // player max fall speed
                    0.1f,                       // player coyote time
                    0.12f,                      // player jump buffer time
                    sf::Color(255, 255, 255),   // player color
                    sf::Vector2f(36.0f, 72.0f), // player size
                    sf::Vector2f(x, y)));       // player position
            }
        }
        column_index++;
    }
}

int main() {
    std::string winTitle = "sfml platformer";
    int winWidth = 1260;
    int winHeight = 900;

    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), winTitle, sf::Style::Close);
    window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - winWidth / 2, sf::VideoMode::getDesktopMode().height / 2 - winHeight / 2));

    std::vector<Player> playerGroup;
    std::vector<SolidTile> solidTileGroup;
    std::vector<OneWayTile> oneWayTileGroup;

    loadLevel(std::string("map.txt"), playerGroup, solidTileGroup, oneWayTileGroup);

    sf::Clock clock;
    float deltaTime;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
        deltaTime = clock.restart().asSeconds();

        for (auto &player : playerGroup) {
            player.update(window, deltaTime, solidTileGroup, oneWayTileGroup);
        }

        window.clear(sf::Color(64, 64, 64));

        for (auto &tile : solidTileGroup) {
            tile.draw(window);
        }
        for (auto &tile : oneWayTileGroup) {
            tile.draw(window);
        }
        for (auto &player : playerGroup) {
            player.draw(window);
        }

        window.display();
    }
    return 0;
}

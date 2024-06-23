#ifndef ONE_WAY_TILE_H
#define ONE_WAY_TILE_H

#include <SFML/Graphics.hpp>

class OneWayTile {
    public:
        sf::RectangleShape rect;
        sf::RectangleShape rectTop;

        OneWayTile(sf::Color color, sf::Vector2f size, sf::Vector2f pos);

        void draw(sf::RenderWindow &window);
};
#endif

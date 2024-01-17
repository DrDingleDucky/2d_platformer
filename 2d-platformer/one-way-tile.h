#ifndef ONEWAYTILE_H
#define ONEWAYTILE_H

#include <sfml/Graphics.hpp>

class OneWayTile {
  public:
    sf::RectangleShape rect;
    sf::RectangleShape rectTop;

    OneWayTile(sf::Color color, sf::Vector2f size, sf::Vector2f pos);

    void draw(sf::RenderWindow &window);
};
#endif

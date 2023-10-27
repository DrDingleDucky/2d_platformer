#ifndef SOLIDTILE_H
#define SOLIDTILE_H

#include <sfml/Graphics.hpp>

class SolidTile {
  public:
    sf::RectangleShape rect;

    SolidTile(sf::Color color, sf::Vector2f size, sf::Vector2f pos);

    void draw(sf::RenderWindow &window);
};
#endif

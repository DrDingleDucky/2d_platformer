#include "solid_tile.h"

SolidTile::SolidTile(sf::Color color, sf::Vector2f size, sf::Vector2f pos) {
    rect.setFillColor(color);
    rect.setSize(size);
    rect.setPosition(pos);
}

void SolidTile::draw(sf::RenderWindow &window) {
    window.draw(rect);
}

#include "one-way-tile.h"

OneWayTile::OneWayTile(sf::Color color, sf::Vector2f size, sf::Vector2f pos) {
    rect.setFillColor(color);
    rect.setSize(size);
    rect.setPosition(pos);
    rectTop.setSize(sf::Vector2f(rect.getSize().x, 1.0f));
    rectTop.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
}

void OneWayTile::draw(sf::RenderWindow &window) {
    window.draw(rect);
}

#include "player.h"

Player::Player(float acceleration, float maxSpeed, float gravity, float jumpVelocity, float fallMultiplier,
               float jumpFallMultiplier, float maxFallSpeed, float coyoteTime, float jumpBufferTime,
               sf::Color color, sf::Vector2f size, sf::Vector2f pos)
    : acceleration(acceleration), maxSpeed(maxSpeed), gravity(gravity), jumpVelocity(jumpVelocity),
      fallMultiplier(fallMultiplier), jumpFallMultiplier(jumpFallMultiplier), maxFallSpeed(maxFallSpeed),
      coyoteTime(coyoteTime), jumpBufferTime(jumpBufferTime), isGrounded(false), holdingSpace(false),
      coyoteTimeTimer(0.0f), jumpBufferTimer(0.0f), direction(sf::Vector2f(0.0f, 0.0f)) {
    rect.setFillColor(color);
    rect.setSize(size);
    rect.setPosition(pos);
    rectBottom.setSize(sf::Vector2f(rect.getSize().x, 1.0f));
    rectBottom.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y + rect.getSize().y - 1.0f));
}

void Player::update(sf::RenderWindow &window, float deltaTime, std::vector<SolidTile> &solidTileGroup, std::vector<OneWayTile> &oneWayTileGroup) {
    rectBottom.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y + rect.getSize().y - 1.0f));

    collisionOneWayTile(oneWayTileGroup);
    horizontalMovement(deltaTime);
    horizontalCollisionsSolidTile(solidTileGroup);
    verticalMovement(deltaTime);
    verticalCollisionsSolidTile(solidTileGroup);
    camera(window);
}

void Player::draw(sf::RenderWindow &window) {
    window.draw(rect);
}

void Player::collisionOneWayTile(std::vector<OneWayTile> &oneWayTileGroup) {
    for (auto &tile : oneWayTileGroup) {
        if (rectBottom.getGlobalBounds().intersects(tile.rectTop.getGlobalBounds())) {
            if (direction.y > 0.0f && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                isGrounded = true;
                direction.y = 0.0f;
                rect.setPosition(sf::Vector2f(rect.getPosition().x, tile.rect.getGlobalBounds().top - rect.getSize().y));
            }
        }
    }
}

void Player::horizontalMovement(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x -= acceleration * deltaTime;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x += acceleration * deltaTime;
    } else {
        int sign = direction.x > 0 ? -1 : 1;

        if (sign * direction.x < 0.0f) {
            direction.x += sign * acceleration * deltaTime;
            if (sign * direction.x > 0.0f) {
                direction.x = 0.0f;
            }
        }
    }

    direction.x = std::clamp(direction.x, -maxSpeed, maxSpeed);
    rect.move(sf::Vector2f(direction.x * deltaTime, 0.0f));
}

void Player::horizontalCollisionsSolidTile(std::vector<SolidTile> &solidTileGroup) {
    for (auto &tile : solidTileGroup) {
        if (rect.getGlobalBounds().intersects(tile.rect.getGlobalBounds())) {
            if (direction.x > 0.0f) {
                direction.x = 0.0f;
                rect.setPosition(sf::Vector2f(tile.rect.getGlobalBounds().left - rect.getSize().x, rect.getPosition().y));
            } else if (direction.x < 0.0f) {
                direction.x = 0.0f;
                rect.setPosition(sf::Vector2f(tile.rect.getGlobalBounds().left + tile.rect.getSize().x, rect.getPosition().y));
            }
        }
    }
}

void Player::verticalMovement(float deltaTime) {
    if (jumpBufferTimer > 0.0f && coyoteTimeTimer > 0.0f) {
        direction.y = jumpVelocity;
        coyoteTimeTimer = 0.0f;
        isGrounded = false;
    } else if (direction.y > 0.0f && isGrounded) {
        isGrounded = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !holdingSpace) {
        jumpBufferTimer = jumpBufferTime;
        holdingSpace = true;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        holdingSpace = false;
    }

    jumpBufferTimer -= deltaTime;

    if (isGrounded) {
        coyoteTimeTimer = coyoteTime;
    }

    coyoteTimeTimer -= deltaTime;

    if (direction.y > maxFallSpeed) {
        direction.y = maxFallSpeed;
    } else if (direction.y > 0.0f) {
        direction.y += gravity * fallMultiplier * deltaTime;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && direction.y < 0.0f) {
        direction.y += gravity * jumpFallMultiplier * deltaTime;
    } else {
        direction.y += gravity * deltaTime;
    }
    rect.move(sf::Vector2f(0, direction.y * deltaTime));
}

void Player::verticalCollisionsSolidTile(std::vector<SolidTile> &solidTileGroup) {
    for (auto &tile : solidTileGroup) {
        if (rect.getGlobalBounds().intersects(tile.rect.getGlobalBounds())) {
            if (direction.y > 0.0f) {
                isGrounded = true;
                direction.y = 0.0f;
                rect.setPosition(sf::Vector2f(rect.getPosition().x, tile.rect.getGlobalBounds().top - rect.getSize().y));
            } else if (direction.y < 0.0f) {
                direction.y = 0.0f;
                rect.setPosition(sf::Vector2f(rect.getPosition().x, tile.rect.getGlobalBounds().top + tile.rect.getSize().y));
            }
        }
    }
}

void Player::followCamera(float horizontalDeadZone, float verticalDeadZone, sf::RenderWindow &window) {
    if (rect.getPosition().x + rect.getSize().x > window.getView().getCenter().x + horizontalDeadZone) {
        window.setView(sf::View(sf::FloatRect(rect.getPosition().x + rect.getSize().x - window.getSize().x / 2.0f - horizontalDeadZone,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().x < window.getView().getCenter().x - horizontalDeadZone) {
        window.setView(sf::View(sf::FloatRect(rect.getPosition().x - window.getSize().x / 2.0f + horizontalDeadZone,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    }

    if (rect.getPosition().y + rect.getSize().y > window.getView().getCenter().y + verticalDeadZone) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f,
                                              rect.getPosition().y + rect.getSize().y - window.getSize().y / 2.0f - verticalDeadZone, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().y < window.getView().getCenter().y - verticalDeadZone) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f,
                                              rect.getPosition().y - window.getSize().y / 2.0f + verticalDeadZone, window.getSize().x, window.getSize().y)));
    }
}

void Player::stationaryCamera(sf::RenderWindow &window) {
    if (rect.getPosition().x + rect.getSize().x / 2.0f < window.getView().getCenter().x - window.getSize().x / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f * 3.0f,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().x + rect.getSize().x / 2.0f > window.getView().getCenter().x + window.getSize().x / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x + window.getSize().x / 2.0f * 3.0f,
                                              window.getView().getCenter().y + window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().y + rect.getSize().y / 2.0f < window.getView().getCenter().y - window.getSize().y / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x + window.getSize().x / 2.0f,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f * 3.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().y + rect.getSize().y / 2.0f > window.getView().getCenter().y + window.getSize().y / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f,
                                              window.getView().getCenter().y + window.getSize().y / 2.0f * 3.0f, window.getSize().x, window.getSize().y)));
    }
}

void Player::horizontalCamera(sf::RenderWindow &window) {
    if (rect.getPosition().y + rect.getSize().y / 2.0f < window.getView().getCenter().y - window.getSize().y / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x + window.getSize().x / 2.0f,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f * 3.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().y + rect.getSize().y / 2.0f > window.getView().getCenter().y + window.getSize().y / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f,
                                              window.getView().getCenter().y + window.getSize().y / 2.0f * 3.0f, window.getSize().x, window.getSize().y)));
    } else {
        window.setView(sf::View(sf::FloatRect(rect.getPosition().x + rect.getSize().x / 2.0f - window.getSize().x / 2.0f,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    }
}

void Player::verticalCamera(sf::RenderWindow &window) {
    if (rect.getPosition().x + rect.getSize().x / 2.0f < window.getView().getCenter().x - window.getSize().x / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f * 3.0f,
                                              window.getView().getCenter().y - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    } else if (rect.getPosition().x + rect.getSize().x / 2.0f > window.getView().getCenter().x + window.getSize().x / 2.0f) {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x + window.getSize().x / 2.0f * 3.0f,
                                              window.getView().getCenter().y + window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    } else {
        window.setView(sf::View(sf::FloatRect(window.getView().getCenter().x - window.getSize().x / 2.0f,
                                              rect.getPosition().y + rect.getSize().y / 2.0f - window.getSize().y / 2.0f, window.getSize().x, window.getSize().y)));
    }
}

void Player::camera(sf::RenderWindow &window) {
    horizontalCamera(window);
}

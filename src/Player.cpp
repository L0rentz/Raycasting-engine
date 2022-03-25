#include "Player.hpp"

Player::Player()
{
    _circle.setRadius(6);
    _circle.setOrigin(sf::Vector2f{_circle.getRadius(), _circle.getRadius()});
    _circle.setFillColor(sf::Color::Yellow);
}

Player::~Player()
{
}

void Player::event(const sf::Event &event, const sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseMoved) {
        _circle.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
    }
}

void Player::update()
{
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(_circle);
}
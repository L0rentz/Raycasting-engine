#include "Player.hpp"

Player::Player(const sf::RenderWindow &window)
{
    _windowSize = window.getSize();
    _circle.setRadius(6);
    _circle.setOrigin(sf::Vector2f{_circle.getRadius(), _circle.getRadius()});
    _circle.setFillColor(sf::Color::Yellow);
    _line = sf::VertexArray(sf::LinesStrip, 2);
    _line[0].color = sf::Color::Green;
    _line[1].color = sf::Color::Green;
    _fov = 90.0f;
    _angle = 0.0f;
}

Player::~Player()
{
}

float Player::magnitude(sf::Vector2f vec2)
{
    return std::sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
}

sf::Vector2f Player::normalize(sf::Vector2f vec2)
{
    float length = magnitude(vec2);
    return sf::Vector2f(vec2.x / length, vec2.y / length);
}

sf::Vector2f Player::rotatePointAroundCenter(sf::Vector2f center, sf::Vector2f point, float angleRad)
{
    float s = sin(angleRad);
    float c = cos(angleRad);

    point.x -= center.x;
    point.y -= center.y;

    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    point.x = xnew + center.x;
    point.y = ynew + center.y;

    return point;
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

sf::Vector2f Player::getPlayerDirection(const float angleRad, const float len) {
    return sf::Vector2f(len * cos(angleRad), len * sin(angleRad));
}

void Player::raycast(const std::map<int, Map::line_t> &vertexmap, const Map::cell_t *cellmap)
{
    _sortedVertex.clear();
    _triangleFan.clear();
    sf::Vector2f rayStart = sf::Vector2f{_circle.getPosition().x / CASE_SIZE, _circle.getPosition().y / CASE_SIZE};
    sf::Vector2i mapCheck = sf::Vector2i{static_cast<int>(rayStart.x), static_cast<int>(rayStart.y)};
    if (mapCheck.x >= 0 && mapCheck.x < _windowSize.x / CASE_SIZE && mapCheck.y >= 0 && mapCheck.y < _windowSize.y / CASE_SIZE) {
        if (cellmap[static_cast<int>(mapCheck.x + mapCheck.y * _windowSize.x / CASE_SIZE)].exist)
            return;
    } else return;

    float offset = 0.0001f;
    for (auto it : vertexmap) {
        float itAngle = atan2f(it.second.A.x - _circle.getPosition().x, it.second.A.y - _circle.getPosition().y);
        _sortedVertex.push_back(std::make_pair(itAngle, it.second.A));
        _sortedVertex.push_back(std::make_pair(itAngle - offset, rotatePointAroundCenter(_circle.getPosition(), it.second.A, offset)));
        _sortedVertex.push_back(std::make_pair(itAngle + offset, rotatePointAroundCenter(_circle.getPosition(), it.second.A, -offset)));
        itAngle = atan2f(it.second.B.x - _circle.getPosition().x, it.second.B.y - _circle.getPosition().y);
        _sortedVertex.push_back(std::make_pair(itAngle, it.second.B));
        _sortedVertex.push_back(std::make_pair(itAngle - offset, rotatePointAroundCenter(_circle.getPosition(), it.second.B, offset)));
        _sortedVertex.push_back(std::make_pair(itAngle + offset, rotatePointAroundCenter(_circle.getPosition(), it.second.B, -offset)));
    }
    std::sort(_sortedVertex.begin(), _sortedVertex.end(),
        [&](std::pair<float, sf::Vector2f> &a, std::pair<float, sf::Vector2f> &b) {
            return (a.first < b.first);
        }
    );

    for (auto it : _sortedVertex) {
        mapCheck = sf::Vector2i{static_cast<int>(rayStart.x), static_cast<int>(rayStart.y)};
        sf::Vector2f rayLength = {0.0f, 0.0f};
        sf::Vector2i step = {0, 0};
        sf::Vector2f rayDir = normalize(sf::Vector2f{it.second.x / CASE_SIZE - rayStart.x, it.second.y / CASE_SIZE - rayStart.y});
        sf::Vector2f rayUnitStepSize = sf::Vector2f{std::sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)), std::sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y))};

        if (rayDir.x < 0) {
            step.x = -1;
            rayLength.x = (rayStart.x - static_cast<float>(mapCheck.x)) * rayUnitStepSize.x;
        } else {
            step.x = 1;
            rayLength.x = (static_cast<float>(mapCheck.x + 1) - rayStart.x) * rayUnitStepSize.x;
        }
        if (rayDir.y < 0) {
            step.y = -1;
            rayLength.y = (rayStart.y - static_cast<float>(mapCheck.y)) * rayUnitStepSize.y;
        } else {
            step.y = 1;
            rayLength.y = (static_cast<float>(mapCheck.y + 1) - rayStart.y) * rayUnitStepSize.y;
        }

        bool tileFound = false;
        float maxDistance = 100.0f;
        float distance = 0.0f;
        while (!tileFound && distance < maxDistance) {
            if (rayLength.x < rayLength.y) {
                mapCheck.x += step.x;
                distance = rayLength.x;
                rayLength.x += rayUnitStepSize.x;
            } else {
                mapCheck.y += step.y;
                distance = rayLength.y;
                rayLength.y += rayUnitStepSize.y;
            }
            // if (magnitude(sf::Vector2f{(rayStart.x + rayDir.x * distance * CASE_SIZE), (rayStart.y + rayDir.y * distance * CASE_SIZE)}) > magnitude(sf::Vector2f{_circle.getPosition().x - it.x, _circle.getPosition().y - it.y}))
            //     break;
            if (mapCheck.x >= 0 && mapCheck.x < _windowSize.x / CASE_SIZE && mapCheck.y >= 0 && mapCheck.y < _windowSize.y / CASE_SIZE) {
                if (cellmap[static_cast<int>(mapCheck.x + mapCheck.y * _windowSize.x / CASE_SIZE)].exist)
                    tileFound = true;
            }
        }
        if (tileFound) {
            _triangleFan.push_back(sf::Vector2f{(rayStart.x + rayDir.x * distance) * CASE_SIZE, (rayStart.y + rayDir.y * distance) * CASE_SIZE});
        }
    }
}

void Player::drawDebug(sf::RenderWindow &window)
{
    _line[0].position = _circle.getPosition();
    for (auto it : _triangleFan) {
        _line[1].position = it;
        window.draw(_line);
    }
}

void Player::draw(sf::RenderWindow &window)
{
    sf::VertexArray triangle = sf::VertexArray(sf::Triangles, 3);
    for (int i = 0; i < 3; i++) triangle[i].color = sf::Color(150, 150, 150, 255);
    triangle[0].position = _circle.getPosition();
    if (_triangleFan.size() >= 3) {
        for (size_t i = 0; i < _triangleFan.size() - 1; i++) {
            triangle[1].position = _triangleFan[i];
            triangle[2].position = _triangleFan[i + 1];
            window.draw(triangle);
        }
        triangle[1].position = _triangleFan[_triangleFan.size() - 1];
        triangle[2].position = _triangleFan[0];
        window.draw(triangle);
    }
    // drawDebug(window);
    window.draw(_circle);
}
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#define PI 3.141592653589793

#include "SFML/Graphics.hpp"
#include "Map.hpp"

class Player {
    public:
        Player(const sf::RenderWindow &window);
        ~Player();

        void event(const sf::Event &event, const sf::RenderWindow &window);
        void update();
        void raycast(const std::map<int, Map::line_t> &vertexmap, const Map::cell_t *cellmap);
        void draw(sf::RenderWindow &window);
    
    protected:
    private:
        sf::Vector2u _windowSize;
        float _fov;
        float _angle;
        sf::CircleShape _circle;
        sf::VertexArray _line;
        std::vector<std::pair<float, sf::Vector2f>> _sortedVertex;
        std::vector<sf::Vector2f> _triangleFan;

        float magnitude(sf::Vector2f vec2);
        sf::Vector2f normalize(sf::Vector2f vec2);
        sf::Vector2f getPlayerDirection(const float angleRad, const float len);
        sf::Vector2f rotatePointAroundCenter(sf::Vector2f center, sf::Vector2f point, float angleRad);
};

#endif /* !PLAYER_HPP_ */

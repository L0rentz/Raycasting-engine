#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "SFML/Graphics.hpp"

class Player {
    public:
        Player();
        ~Player();

        void event(const sf::Event &event, const sf::RenderWindow &window);
        void update();
        void draw(sf::RenderWindow &window);
    
    protected:
    private:
        sf::CircleShape _circle;
};

#endif /* !PLAYER_HPP_ */

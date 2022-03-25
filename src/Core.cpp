#include "Core.hpp"

Core::Core()
{
    _window.create(sf::VideoMode(1920, 1080), "Raycasting Engine");
    _window.setMouseCursorVisible(false);
    _maps.push_back(Map(MAP_PATH + std::string("test.txt"), _window));
    _selected = 0;

}

Core::~Core()
{
}

void Core::event()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            _window.close();
        _player.event(_event, _window);
        _maps[_selected].event(_event, _window);
    }
}

void Core::update()
{
}

void Core::display()
{
    _window.clear();
    _maps[_selected].draw(_window);
    _player.draw(_window);
    _window.display();
}

void Core::run()
{
    while (_window.isOpen()) {
        event();
        update();
        display();
    }
}
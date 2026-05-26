#pragma once

#include "Constants.h"
#include <SFML/Graphics.hpp>

enum InputCommand { None, Default, Mouse0Down, Mouse0Up, Mouse1Down, Mouse1Up, 
    Mouse0DownDouble, Mouse0UpDouble, Mouse1DownDouble, Mouse1UpDouble,
    Confirm, Cancel };

struct MouseState {
    bool leftDown = false;
    bool rightDown = false;
    sf::Vector2i position;
};

class Input {
public:
    Input();

    InputCommand GetCommand(sf::RenderWindow& window) const;
    MouseState GetMouseState(sf::RenderWindow& window) const;

private:
    mutable sf::Vector2i lastClickPos = {-1000, -1000};
    mutable std::uint64_t lastLeftClickTime = 0;
    mutable std::uint64_t lastRightClickTime = 0;
    mutable bool lastLeftDown = false;
    mutable bool lastRightDown = false;
};
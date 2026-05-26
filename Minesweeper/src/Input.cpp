#include "Input.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
// 双击判定参数
#include <chrono>

Input::Input() {
    // 构造函数，如果需要初始化可以在这里
}

InputCommand Input::GetCommand(sf::RenderWindow& window) const {
    InputCommand cmd = InputCommand::None;
    using namespace std::chrono;
    auto now = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    const int DOUBLE_CLICK_TIME = 300; // ms
    const int DOUBLE_CLICK_DIST = 5; // px

    MouseState state = GetMouseState(window);
    // 检测左键单击
    if (state.leftDown && !lastLeftDown) {
        // 判断与上次点击的时间和距离
        if (now - lastLeftClickTime < DOUBLE_CLICK_TIME &&
            std::abs(state.position.x - lastClickPos.x) <= DOUBLE_CLICK_DIST &&
            std::abs(state.position.y - lastClickPos.y) <= DOUBLE_CLICK_DIST) {
            cmd = Mouse0DownDouble;
            lastLeftClickTime = 0; // 重置，防止连击
        } else {
            cmd = Mouse0Down;
            lastLeftClickTime = now;
            lastClickPos = state.position;
        }
    } else {
        cmd = InputCommand::None;
    }
    lastLeftDown = state.leftDown;

    if(state.rightDown && !lastRightDown){
        if(now - lastRightClickTime < DOUBLE_CLICK_TIME &&
            std::abs(state.position.x - lastClickPos.x) <= DOUBLE_CLICK_DIST &&
            std::abs(state.position.y - lastClickPos.y) <= DOUBLE_CLICK_DIST) {
            cmd = Mouse1DownDouble;
            lastRightClickTime = 0; // 重置，防止连击
        } else {
            cmd = Mouse1Down;
            lastRightClickTime = now;
            lastClickPos = state.position;
        }
    }
    lastRightDown = state.rightDown;
    return cmd;
}

MouseState Input::GetMouseState(sf::RenderWindow& window) const {
    MouseState state;
    state.position = sf::Mouse::getPosition(window);
    state.leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    state.rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    return state;
}
#pragma once

#include "Constants.h"
#include "TimeSystem.h"
#include "Renderer.h"
#include "Input.h"
#include "Scene.h"
#include "ResourceManager.h"
#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace sf;

class Game {
public:
    Game();
    void Run();

private:
    int windowWidth, windowHeight;
    RenderWindow window;

    GameState gameState;
    Diff diff;
    int gridWidth, gridHeight;
    int mineNum;
    Scene grid;
    int gridSpriteSize;
    sf::Vector2f gridStartPos;

    Vector2i BtnLeftCorner;
    int BtnDetaX;
    std::vector<Button_my> btns;

    ResourceManager rm;
    Renderer renderer;
    Input input;
    TimeSystem timeSystem;
    float intervalTime;
    float intervalTimer;

    int curBackground;
    int curSkin;
    float timer;

    bool win = false;

    void Init();
    void Logic();
    void HandleInput();
    void Draw();

    void ChangeDifficulty(Diff d);
    void Restart();
    void NextBackground();
    void NextSkin();
    void HandleMouse(const MouseState& mouse);
    void HandleButtonClick(ButtonType type);
    void UpdateTimer(float dt);
};

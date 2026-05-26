#include "Game.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <thread>
#include <math.h>

Game::Game():gridSpriteSize(GRIDSIZE * 2), renderer(rm){
    windowWidth = 1920;
    windowHeight = 1080;
    uint8_t WindowStyle = sf::Style::Close | sf::Style::Titlebar;
    window.create(sf::VideoMode({
            static_cast<unsigned int>(windowWidth),
            static_cast<unsigned int>(windowHeight)
        }), L"MineSweeper", WindowStyle);

    BtnDetaX = (windowWidth - BTN_WIDTH * 7) / 8;
    BtnLeftCorner = {BtnDetaX, windowHeight - GRIDSIZE * 3};

    timeSystem = TimeSystem();
    intervalTime = 0.5f;
    intervalTimer = 0.0f;
    gameState = GameState::MENU;
    diff = Diff::EASY;
    curBackground = 0;
    curSkin = 0;
    timer = 0.0f;

    auto d = DiffMap.at(diff);
    gridWidth = d.width;
    gridHeight = d.height;
    mineNum = d.mineNum;
    grid.Reset(gridWidth, gridHeight, mineNum);

    renderer.Init();
}

void Game::Run(){
    Init();
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS; // ms
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        HandleInput();
        Logic();
        Draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));
    }
}

void Game::Init(){
    win = false;
    // 初始化按钮
    btns.clear();
    int x = BtnDetaX;
    for(int i = 0; i < 7; i++){
        ButtonType type = static_cast<ButtonType>(i);
        sf::IntRect rect({x, BtnLeftCorner.y}, sf::Vector2i(BTN_WIDTH, BTN_HEIGHT));
        btns.emplace_back(type, rect);
        x += BtnDetaX + BTN_WIDTH;
    }
    // 初始化游戏状态
    auto d = DiffMap.at(diff);
    gridWidth = d.width;
    gridHeight = d.height;
    mineNum = d.mineNum;
    grid.Reset(gridWidth, gridHeight, mineNum);
    gridStartPos.x = (window.getSize().x - gridWidth * gridSpriteSize) / 2;
    gridStartPos.y = (window.getSize().y - gridHeight * gridSpriteSize) / 2;
    timer = 0.0f;
    gameState = GameState::MENU;
}

void Game::Logic(){
    float dt = timeSystem.GetDeltaTime();
    // 判胜负
    if (gameState == GameState::RUNNING) {
        // 更新计时器
        timer += dt;
        if (grid.GetOpenCount() == grid.GetWidth() * grid.GetHeight() - grid.GetMineNum()) {
            win = true;
            gameState = GameState::GAMEOVER;
            grid.RevealAll();
        }
    }
}

void Game::HandleInput(){
    MouseState mouse = input.GetMouseState(window);
    // 按钮 hover
    for(auto& b : btns){
        if(b.rect.contains(mouse.position)){
            b.state = ButtonState::Hover;
        }
        else{
            b.state = ButtonState::Default;
        }
    }

    InputCommand cmd = input.GetCommand(window);

    // 通过 Input 类统一处理输入
    int gy = (mouse.position.x - gridStartPos.x) / gridSpriteSize;
    int gx = (mouse.position.y - gridStartPos.y) / gridSpriteSize;
    
    switch (cmd)
    {
    // 鼠标左键
    case Mouse0Down:
        // 检查按钮
        for (auto& b : btns) {
            if (b.rect.contains(mouse.position)) {
                HandleButtonClick(b.type);
                return;
            }
        }
        // 检查格子
        if (grid.IsInBounds(gx,gy)) {
            // 首次点击后才开始计时
            if (gameState == GameState::MENU){
                gameState = GameState::RUNNING;
            }
            bool isMine = grid.Reveal(gx, gy);
            if (isMine) {
                win = false;
                gameState = GameState::GAMEOVER;
            }
        }
        break;
    // 鼠标右键
    case Mouse1Down:
        if (grid.IsInBounds(gx, gy)) {
            grid.ToggleFlag(gx, gy);
        }
        break;
    // 鼠标左键双击
    case Mouse0DownDouble:
        if (grid.IsInBounds(gx, gy)) {
            grid.QuickOpen(gx, gy);
        }
        break;
    default:
        break;
    }
}

void Game::Draw(){
    window.clear();
    renderer.DrawBackground(window, curBackground);
    renderer.DrawGrid(window, grid, gridStartPos, gridSpriteSize, curSkin);
    renderer.DrawButton(window, BtnLeftCorner, BtnDetaX, btns);
    renderer.DrawScore(window, fmax(0, grid.GetMineNum() - grid.GetFlagCount()));
    renderer.DrawTimer(window, fmin(timer, 999.9f));
    if (gameState == GameState::GAMEOVER) {
        renderer.DrawGameEnd(window, win);
    }
    window.display();
}

void Game::ChangeDifficulty(Diff d) {
    diff = d;
    Init();
}

void Game::Restart() {
    Init();
}

void Game::NextBackground() {
    int cnt = rm.getTextureCount(TextureType::BACKGROUND);
    curBackground = (curBackground + 1) % cnt;
}

void Game::NextSkin() {
    int cnt = rm.getTextureCount(TextureType::GRID);
    curSkin = (curSkin + 1) % cnt;
}

void Game::HandleButtonClick(ButtonType type) {
    switch (type) {
        case ButtonType::Easy:
            ChangeDifficulty(Diff::EASY);
            break;
        case ButtonType::Normal:
            ChangeDifficulty(Diff::MIDDLE);
            break;
        case ButtonType::Hard:
            ChangeDifficulty(Diff::DIFFICULT);
            break;
        case ButtonType::BG:
            NextBackground();
            break;
        case ButtonType::Skin:
            NextSkin();
            break;
        case ButtonType::Restart:
            Restart();
            break;
        case ButtonType::Quit:
            window.close();
            break;
        default:
            break;
    }
}
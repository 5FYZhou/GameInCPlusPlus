#include "Game.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>

Game::Game()
    : gameState(GameState::GAMEOVER)
    , isPausedExit(false)
    , h(GetStdHandle(STD_OUTPUT_HANDLE))
    , renderer(h)
{
}

void Game::Run() {
    renderer.hideCursor();
    if (SaveSystem::HasSaveData()) {
        InitPaused();
    } else {
        Init();
    }

    renderer.drawScene(scene);
    while (gameState != GameState::GAMEOVER) {
        switch (gameState) {
        case GameState::RUNNING:
            renderer.drawScore(gameData.score);
            renderer.drawScene(scene);
            Sleep(DELAY);
            Input();
            Logic();
            break;
        case GameState::PAUSED:
            renderer.drawPause();
            renderer.drawScore(gameData.score);
            Sleep(DELAY);
            InputPause();
            LogicPause();
            break;
        default:
            break;
        } 
    }

    if (isPausedExit) {
        SaveSystem::Save(gameData);
    } else {
        SaveSystem::ClearSaveData();
    }

    renderer.gotoXY(0, SCENE_HEIGHT + 1);
    system("pause");
}

void Game::SetShowMode(ShowMode mode) {
    renderer.setCurShowMode(mode);
    gameData.mode = mode;
}

void Game::ChangeShowMode() {
    if (gameData.mode == ShowMode::FULL) {
        SetShowMode(ShowMode::HALF);
    } else {
        SetShowMode(ShowMode::FULL);
    }
    system("cls");
    renderer.drawScene(scene);
}

void Game::Init() {
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    gameState = GameState::RUNNING;
    SetShowMode(ShowMode::FULL);
    srand(static_cast<unsigned int>(time(nullptr)));

    ResetScene();

    gameData.snake.clear();
    gameData.snake.push_back({ WIDTH / 2, HEIGHT / 2 });
    SetGameScene(gameData.snake[0], CodeType::SNAKE);

    gameData.food = GetFoodPos();
    SetGameScene(gameData.food, CodeType::FOOD);
    gameData.score = 0;
}

void Game::Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            if (gameData.dir != DOWN) {
                gameData.dir = UP;
            }
            break;
        case 's':
            if (gameData.dir != UP) {
                gameData.dir = DOWN;
            }
            break;
        case 'a':
            if (gameData.dir != RIGHT) {
                gameData.dir = LEFT;
            }
            break;
        case 'd':
            if (gameData.dir != LEFT) {
                gameData.dir = RIGHT;
            }
            break;
        case 27:
            gameState = GameState::GAMEOVER;
            break;
        case 32:
            gameState = GameState::PAUSED;
            break;
        case 13:
            ChangeShowMode();
            break;
        default:
            break;
        }
    }
}

void Game::Logic() {
    if (gameData.dir == Direction::STOP) {
        return;
    }

    Vector2 newHead = gameData.snake[0] + DIR_MAP.at(gameData.dir);
    if (Check(newHead, CodeType::WALL) || Check(newHead, CodeType::SNAKE)) {
        gameState = GameState::GAMEOVER;
        return;
    }

    gameData.snake.insert(gameData.snake.begin(), newHead);
    if (newHead == gameData.food) {
        gameData.food = GetFoodPos();
        SetGameScene(gameData.food, CodeType::FOOD);
        gameData.score += 1;
    } 
    else {
        Vector2 tail = gameData.snake.back();
        SetGameScene(tail, CodeType::DEFAULT);
        gameData.snake.pop_back();
    }
    SetGameScene(newHead, CodeType::SNAKE);
}

void Game::InitPaused() {
    system("cls");
    std::cout << "继续游戏？（Y/n）";
    char a = _getch();
    if (a == 'Y' || a == 'y') {
        if (SaveSystem::Load(gameData)) {
            SetShowMode(gameData.mode);
            ResetScene();
            for (const auto& pos : gameData.snake) {
                SetGameScene(pos, CodeType::SNAKE);
            }
            SetGameScene(gameData.food, CodeType::FOOD);
            gameState = GameState::PAUSED;
        } 
        else {
            SaveSystem::ClearSaveData();
            Init();
            gameState = GameState::RUNNING;
        }
    } 
    else {
        Init();
        gameState = GameState::RUNNING;
    }
}

void Game::InputPause() {
    if (_kbhit()) {
        switch (_getch()) {
        case 27:
            isPausedExit = true;
            gameState = GameState::GAMEOVER;
            break;
        case 13:
            ChangeShowMode();
            break;
        default:
            gameState = GameState::RUNNING;
            renderer.drawScene(scene);
            break;
        }
    }
}

void Game::LogicPause() {
}
